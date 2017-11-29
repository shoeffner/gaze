#include "gaze/util/dlibcv.h"


namespace gaze {

namespace util {

dlib::point cv_to_dlib(cv::Point to_convert) {
  return {to_convert.x, to_convert.y};
}

dlib::rectangle cv_to_dlib(cv::Rect to_convert) {
  return {to_convert.x, to_convert.y,
          to_convert.width, to_convert.height};
}

cv::Rect dlib_to_cv(dlib::rectangle to_convert) {
  return cv::Rect(to_convert.left(), to_convert.top(),
                  to_convert.width(), to_convert.height());
}

}  // namespace util

}  // namespace gaze
