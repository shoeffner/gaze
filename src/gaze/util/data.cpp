// Copyright 2017 Sebastian Höffner

#include "gaze/util/data.h"


namespace gaze {

namespace util {

Data::Data(const cv::Mat raw_capture_image) {
  this->raw_capture_image = raw_capture_image;
}

}  // namespace util

}  // namespace gaze
