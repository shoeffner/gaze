#ifndef INCLUDE_GAZE_UTIL_DLIBCV_H_
#define INCLUDE_GAZE_UTIL_DLIBCV_H_

#include <vector>

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
dlib::point cv_to_dlib(const cv::Point& to_convert);

/**
 * Converts a cv::Point to a dlib::point.
 *
 * @param to_convert The point to convert.
 * @returns The converted point.
 */
dlib::point cv_to_dlib(const cv::Vec3d& to_convert);

template<typename T, int N>
dlib::vector<T, N> cv_to_dlib(const cv::Vec<T, N>& to_convert) {
  return dlib::vector<T, N>(to_convert[0], to_convert[1], to_convert[2]);
}

template<typename T, int N>
cv::Vec<T, N> dlib_to_cv(const dlib::vector<T, N>& to_convert) {
  return cv::Vec<T, N>(to_convert.x(), to_convert.y(), to_convert.z());
}

/**
 * Converts a cv::Rect to a dlib::rectangle.
 *
 * @param to_convert The rectangle to convert.
 * @returns The converted rectangle.
 */
dlib::rectangle cv_to_dlib(const cv::Rect& to_convert);

/**
 * Converts a dlib::rectangle to a cv::Rect.
 *
 * @param to_convert The rectangle to convert.
 * @returns The converted rectangle.
 */
cv::Rect dlib_to_cv(const dlib::rectangle& to_convert);

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_DLIBCV_H_
