// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/pupil_localization.h"

#include <algorithm>
#include <limits>
#include <vector>

#include "dlib/image_processing.h"
#include "dlib/opencv.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "gaze/util/data.h"
#include "gaze/util/utility.h"


namespace gaze {

namespace pipeline {

void PupilLocalization::fill_displacement_tables(
    dlib::matrix<double>& table_x,
    dlib::matrix<double>& table_y,
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

PupilLocalization::PupilLocalization() {
  this->name = "Eye center";
  // Pre-calculate displacement table
  // TODO(shoeffner): Evaluate which size is useful
  this->fill_displacement_tables(
      this->displacement_table_x,
      this->displacement_table_y,
      131);
}

void PupilLocalization::process(util::Data& data) {
  if (data.landmarks.num_parts() != 5) {
    return;
  }

  std::vector<dlib::chip_details> details =
    util::get_eyes_chip_details(data.landmarks);
  dlib::extract_image_chips(data.image, details, data.eyes);
  int max_size = std::max({data.eyes[0].nr(), data.eyes[0].nc(),
                           data.eyes[1].nr(), data.eyes[1].nc()});
  if (2 * max_size > this->displacement_table_x.nr()) {
    // TODO(shoeffner): Instead of +1 use +N to prepare for some more variation
    this->fill_displacement_tables(
        this->displacement_table_x,
        this->displacement_table_y,
        2 * max_size + 1);
  }
}

void PupilLocalization::visualize(util::Data& data) {
  if (data.landmarks.num_parts() != 5) {
    return;
  }

  // define offsets
  dlib::point offset_x(5, 0);
  dlib::point offset_y(0, 5);

  // assign eyes to new images
  dlib::array<dlib::array2d<dlib::bgr_pixel>> eyes(2);
  for (unsigned long i = 0; i < data.eyes.size(); ++i) {
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
