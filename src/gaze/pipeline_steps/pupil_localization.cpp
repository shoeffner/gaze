#include "gaze/pipeline_steps/pupil_localization.h"

#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include "dlib/image_processing.h"
#include "dlib/image_transforms.h"
#include "dlib/matrix.h"
#include "dlib/opencv.h"
#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"
#include "gaze/util/pipeline_utils.h"


namespace gaze {

namespace util {

void fill_displacement_tables(
    dlib::matrix<double>& table_x,  // NOLINT
    dlib::matrix<double>& table_y,  // NOLINT
    int size) {
  // Ensure size is bigger than the current size (i.e. if an update is needed
  // at all)
  if (size <= table_x.nr()) {
    return;
  }

  // Ensure size is odd
  size += (1 - size & 1);
  int half_size = (size - 1) / 2;

  // Resize tables
  table_x.set_size(size, size);
  table_y.set_size(size, size);

  // Fill with indices
  for (int row = 0; row < table_x.nr(); ++row) {
    dlib::set_rowm(table_x, row) = dlib::range(-half_size, half_size);
  }
  table_y = dlib::trans(table_x);

  // Calculate norms and store to table_x, then transpose to table_y
  const double epsilon = std::numeric_limits<double>::min();
  for (int row = 0; row < table_x.nr(); ++row) {
    for (int col = 0; col < table_x.nc(); ++col) {
      table_x(row, col) = table_x(row, col) /
        (std::sqrt(table_x(row, col) * table_x(row, col)
                   + table_y(row, col) * table_y(row, col))
         // Avoid div0
         + epsilon);
    }
  }
  table_y = dlib::trans(table_x);
}

}  // namespace util

namespace pipeline {

PupilLocalization::PupilLocalization()
    // See
    // http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/
    // for parameters
    : relative_threshold_factor(0.3),
      sigma(1.1) {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "PupilLocalization";

  if (config["relative_threshold"]) {
    this->relative_threshold_factor =
      config["relative_threshold"]
        .as<decltype(this->relative_threshold_factor)>();
  }

  // Pre-calculate displacement table
  util::fill_displacement_tables(
      this->displacement_table_x,
      this->displacement_table_y,
      config["table_size"] ? config["table_size"].as<int>() : 131);
}

void PupilLocalization::process(util::Data& data) {
  if (data.landmarks.num_parts() < 5) {
    return;
  }

  // Extract eye patches
  std::vector<dlib::chip_details> details =
    util::get_eyes_chip_details(data.landmarks);
  dlib::extract_image_chips(data.image, details, data.eyes);

  // Resize lookup table if needed
  int max_size = std::max({data.eyes[0].nr(), data.eyes[0].nc(),
                           data.eyes[1].nr(), data.eyes[1].nc()});
  if (2 * max_size > this->displacement_table_x.nr()) {
    util::fill_displacement_tables(
        this->displacement_table_x,
        this->displacement_table_y,
        2 * max_size + 7);
  }

  for (int i = 0; i < 2; ++i) {
    dlib::matrix<double> eye_in;
    dlib::assign_image(eye_in, data.eyes[i]);

    // Calculate gradients
    dlib::matrix<double> eye_horizontal;
    dlib::matrix<double> eye_vertical;
    dlib::sobel_edge_detector(eye_in, eye_horizontal, eye_vertical);

    util::normalize_and_threshold_gradients(eye_horizontal, eye_vertical,
        this->relative_threshold_factor);

    // Blur eyes as weights
    int boundary_offset = 6;
    dlib::matrix<double> eye_gaussian;
    dlib::gaussian_blur(eye_in, eye_gaussian, this->sigma, boundary_offset);
    eye_gaussian = 255 - eye_gaussian;

    // Compute objective function t and select highest value
    double max_t = std::numeric_limits<double>::min();
    int argmax_r;
    int argmax_c;

    int center = (this->displacement_table_x.nr() - 1) / 2;
    auto nr = eye_in.nr();
    auto nc = eye_in.nc();
    dlib::matrix<double> zero = dlib::zeros_matrix<double>(nr, nc);
    dlib::matrix<double> ts = dlib::zeros_matrix<double>(nr, nc);
    for (auto row = decltype(nr){boundary_offset};
         row < nr - boundary_offset; ++row) {
      for (auto col = decltype(nc){boundary_offset};
           col < nc - boundary_offset; ++col) {
        dlib::matrix<double> d_x = dlib::subm(this->displacement_table_x,
            center - row, center - col, nr, nc);
        dlib::matrix<double> d_y = dlib::subm(this->displacement_table_y,
            center - row, center - col, nr, nc);

        double t =
          dlib::sum(
              dlib::squared(dlib::max_pointwise(
                dlib::pointwise_multiply(d_x, eye_horizontal) +
                dlib::pointwise_multiply(d_y, eye_vertical), zero))) *
          eye_gaussian(row, col);
        if (t > max_t) {
          max_t = t;
          argmax_r = row;
          argmax_c = col;
        }
      }
    }
    data.centers[i] = dlib::point(argmax_c, argmax_r);
  }
}

void PupilLocalization::visualize(util::Data& data) {
  if (data.landmarks.num_parts() < 5) {
    return;
  }
  // Ensures output of this pipeline step is visualized
  this->process(data);

  // define offsets
  dlib::point offset_x(5, 0);
  dlib::point offset_y(0, 5);
  std::vector<dlib::chip_details> chips =
    util::get_eyes_chip_details(data.landmarks);

  // assign eyes to new images
  dlib::array<dlib::array2d<dlib::bgr_pixel>> eyes(3);
  dlib::assign_image(eyes[2], data.image);
  for (auto i = decltype(data.eyes.size()){0};
       i < data.eyes.size(); ++i) {
    dlib::assign_image(eyes[i], data.eyes[i]);

    dlib::draw_line(eyes[i],
                    data.centers[i] + offset_x,
                    data.centers[i] - offset_x,
                    dlib::rgb_pixel(255, 0, 0));
    dlib::draw_line(eyes[i],
                    data.centers[i] + offset_y,
                    data.centers[i] - offset_y,
                    dlib::rgb_pixel(255, 0, 0));

    dlib::draw_line(eyes[2],
                    data.centers[i] + offset_x + chips[i].rect.tl_corner(),
                    data.centers[i] - offset_x + chips[i].rect.tl_corner(),
                    dlib::rgb_pixel(255, 0, 0));
    dlib::draw_line(eyes[2],
                    data.centers[i] + offset_y + chips[i].rect.tl_corner(),
                    data.centers[i] - offset_y + chips[i].rect.tl_corner(),
                    dlib::rgb_pixel(255, 0, 0));
  }
  dlib::resize_image(0.5, eyes[2]);

  this->widget->set_image(dlib::tile_images(eyes));
}

}  // namespace pipeline

}  // namespace gaze
