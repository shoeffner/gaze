// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/pupil_localization.h"

#include <vector>

#include "dlib/image_processing.h"
#include "dlib/opencv.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "gaze/util/data.h"
#include "gaze/util/utility.h"


namespace gaze {

namespace pipeline {

PupilLocalization::PupilLocalization() {
  this->name = "Eye center";
}

void PupilLocalization::process(util::Data& data) {
  if (data.landmarks.num_parts() != 5) {
    return;
  }

  std::vector<dlib::chip_details> details =
    util::get_eyes_chip_details(data.landmarks);
  dlib::extract_image_chips(data.image, details, data.eyes);
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
