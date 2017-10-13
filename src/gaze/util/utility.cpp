// Copyright 2017 Sebastian Höffner

#include "gaze/util/utility.h"


#include "dlib/opencv.h"
#include "opencv2/core.hpp"


namespace gaze {

namespace util {

dlib::rectangle convert(cv::Rect in) {
  return dlib::rectangle(in.x,
                         in.y + in.height,
                         in.x + in.width,
                         in.y);
}

cv::Rect convert(dlib::rectangle in) {
  return cv::Rect(in.tl_corner().x(),
                  in.tl_corner().y(),
                  in.width(),
                  in.height());
}

}  // namespace util

}  // namespace gaze
