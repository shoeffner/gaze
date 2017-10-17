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
    this->widget->set_image(data.image);
    return;
  }
  this->widget->set_image(data.image);
}

}  // namespace pipeline

}  // namespace gaze
