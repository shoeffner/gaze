#ifndef INCLUDE_GAZE_UTIL_DLIBCV_H_
#define INCLUDE_GAZE_UTIL_DLIBCV_H_

#include "opencv2/opencv.hpp"
#include "dlib/geometry.h"


namespace gaze {

namespace util {

/**
 * Converts a cv::Point to a dlib::point.
 *
 * @param to_convert The point to convert.
 * @returns The converted point.
 */
dlib::point cv_to_dlib(cv::Point to_convert);

/**
 * Converts a cv::Rect to a dlib::rectangle.
 *
 * @param to_convert The rectangle to convert.
 * @returns The converted rectangle.
 */
dlib::rectangle cv_to_dlib(cv::Rect to_convert);

/**
 * Converts a dlib::rectangle to a cv::Rect.
 *
 * @param to_convert The rectangle to convert.
 * @returns The converted rectangle.
 */
cv::Rect dlib_to_cv(dlib::rectangle to_convert);


}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_DLIBCV_H_
