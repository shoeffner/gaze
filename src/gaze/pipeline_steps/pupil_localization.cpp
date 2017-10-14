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

  std::vector<dlib::chip_details> chips =
    util::get_eyes_chip_details(data.landmarks);
  for (dlib::chip_details eye_bb : chips) {
    cv::rectangle(data.source_image,
                  util::crop_to_image_boundary(
                    data.source_image,
                    util::convert(eye_bb.rect)),
                  cv::Scalar(0, 255, 255));
  }

  cv::imshow("left eye", dlib::toMat(data.eyes[0]));
  cv::imshow("right eye", dlib::toMat(data.eyes[1]));
}

}  // namespace pipeline

}  // namespace gaze
