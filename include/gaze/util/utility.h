// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_UTILITY_H_
#define INCLUDE_GAZE_UTIL_UTILITY_H_

#include "dlib/opencv.h"
#include "opencv2/core.hpp"


namespace gaze {

namespace util {

// TODO(shoeffner): Add unit tests for utility.h

/** @name OpenCV/dlib Conversions
 *
 * Convenience functions to convert between OpenCV and dlib which are not built-in.
 */
//@{
/**
 * Converts the between Rect and rectangle.
 *
 * @param in Rect to convert.
 * @returns converted rectangle.
 */
dlib::rectangle convert(cv::Rect in);

/**
 * Converts the between Rect and rectangle.
 *
 * @param in rectangle to convert.
 * @returns converted Rect.
 */
cv::Rect convert(dlib::rectangle in);
//@}

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_UTILITY_H_
