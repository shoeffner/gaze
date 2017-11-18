// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/pupil_localization.h"

#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include "dlib/image_processing.h"
#include "dlib/image_transforms.h"
#include "dlib/matrix.h"
#include "dlib/opencv.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"


namespace gaze {

namespace util {

std::vector<dlib::chip_details> get_eyes_chip_details(
    const dlib::full_object_detection object_detection) {
  std::vector<dlib::chip_details> details;
  if (object_detection.num_parts() < 5) {
    return details;
  }

  auto get_rectangle = [](dlib::point one, dlib::point two)
    -> dlib::rectangle {
      dlib::rectangle result(one, two);
      double scale = (one - two).length() * 1.5;
      return dlib::centered_rect(result, scale, scale);
    };

  int index_ex_left;
  int index_en_left;
  int index_ex_right;
  int index_en_right;
  if (object_detection.num_parts() == 5) {
    index_ex_left = 0;
    index_en_left = 1;
    index_ex_right = 2;
    index_en_right = 3;
  } else {  // 68 landmarks
    index_ex_left = 45;
    index_en_left = 42;
    index_ex_right = 36;
    index_en_right = 39;
  }

  details.push_back(dlib::chip_details(
        get_rectangle(object_detection.part(index_ex_left),
                      object_detection.part(index_en_left))));
  details.push_back(dlib::chip_details(
        get_rectangle(object_detection.part(index_ex_right),
                      object_detection.part(index_en_right))));

  return details;
}

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
      sigma(-1),
      sigma_factor(0.005) {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "PupilLocalization";

  if (config["relative_threshold"]) {
    this->relative_threshold_factor =
      config["relative_threshold"]
        .as<decltype(this->relative_threshold_factor)>();
  }
  if (config["sigma"]) {
    this->sigma = config["sigma"].as<decltype(this->sigma)>();
  }
  if (config["sigma_factor"]) {
    this->sigma_factor =
      config["sigma_factor"].as<decltype(this->sigma_factor)>();
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

  std::vector<dlib::chip_details> details =
    util::get_eyes_chip_details(data.landmarks);
  dlib::extract_image_chips(data.image, details, data.eyes);
  int max_size = std::max({data.eyes[0].nr(), data.eyes[0].nc(),
                           data.eyes[1].nr(), data.eyes[1].nc()});
  if (2 * max_size > this->displacement_table_x.nr()) {
    // TODO(shoeffner): Instead of +1 use +N to prepare for some more variation
    util::fill_displacement_tables(
        this->displacement_table_x,
        this->displacement_table_y,
        2 * max_size + 1);
  }

  for (int i = 0; i < 2; ++i) {
    // Assign image and scale to 0..1
    dlib::matrix<double> eye_in;
    dlib::assign_image(eye_in, data.eyes[i]);
    eye_in /= 255;

    // Blur image
    dlib::matrix<double> eye_gaussian;
    dlib::gaussian_blur(eye_in, eye_gaussian,
        this->sigma >= 0 ? this->sigma :
        this->sigma_factor * data.landmarks.get_rect().height());

    // Calculate gradients
    dlib::matrix<double> eye_horizontal;
    dlib::matrix<double> eye_vertical;
    dlib::sobel_edge_detector(eye_gaussian, eye_horizontal, eye_vertical);
    eye_horizontal *= -1;
    // eye_vertical *= -1;
    util::normalize_and_threshold_gradients(eye_horizontal, eye_vertical,
        this->relative_threshold_factor);

    // Compute objective function t - only implicit
    double max_t = std::numeric_limits<double>::min();
    int argmax_r;
    int argmax_c;

    int size = this->displacement_table_x.nr();
    int center = (size - 1) / 2;
    int nr = eye_in.nr();
    int nc = eye_in.nc();
    // TODO(shoeffner): Consider smaller range
    for (int row = 0; row < nr; ++row) {
      for (int col = 0; col < nc; ++col) {
        dlib::matrix<double> d_x = dlib::subm(this->displacement_table_x,
            center - row, center - col, nr, nc);
        dlib::matrix<double> d_y = dlib::subm(this->displacement_table_y,
            center - row, center - col, nr, nc);

        double t =
          dlib::mean(
            dlib::squared(
              // TODO(shoeffner): Evaluate abs and non-flipped horizontal
              // gradient
              dlib::max_pointwise(
                dlib::pointwise_multiply(d_x, eye_horizontal) +
                dlib::pointwise_multiply(d_y, eye_vertical),
                dlib::zeros_matrix<double>(nr, nc))));
        if (t > max_t) {
          max_t = t;
          argmax_r = row;
          argmax_c = col;
        }
      }
    }
    data.centers[i] = dlib::point(argmax_r, argmax_c);
  }
}

void PupilLocalization::visualize(util::Data& data) {
  if (data.landmarks.num_parts() < 5) {
    return;
  }

  // define offsets
  dlib::point offset_x(5, 0);
  dlib::point offset_y(0, 5);

  // assign eyes to new images
  dlib::array<dlib::array2d<dlib::bgr_pixel>> eyes(2);
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
  }

  this->widget->set_image(dlib::tile_images(eyes));
}

}  // namespace pipeline

}  // namespace gaze
