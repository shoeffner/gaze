// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_UTILITY_H_
#define INCLUDE_GAZE_UTIL_UTILITY_H_

#include <string>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"


namespace gaze {

namespace util {

// TODO(shoeffner): Add unit tests for utility.h

/**
 * Helper function to load classifiers by path.
 * Prints to STDERR if a classifier can not be loaded.
 *
 * @param file_name The cascade filename to load the xml from.
 * @returns the CascadeClassifier, might not be loaded.
 */
cv::CascadeClassifier load_classifier(std::string file_name);

/** @name Left to right order tests. */
//@{
/**
 * Checks if left's x value is less than right.
 *
 * @param left A rectangle to test.
 * @param right A value which should be right of left.x.
 * @returns true if left is left of right.
 */
const bool ltr(const cv::Rect left, const double right);

/**
 * Checks if left's x value is less than right's.
 *
 * @param left The left rectangle.
 * @param right The right rectangle.
 * @returns true if left is left of right (i.e. its x-value is less than
 *          right's)
 */
const bool ltr(const cv::Rect left, const cv::Rect right);

/**
 * Checks if left is less than right's x value.
 *
 * @param left A value which should be left of right.x.
 * @param right A rectangle to test.
 * @returns true if left is left of right.
 */
const bool ltr(const double left, const cv::Rect right);

/**
 * Checks if the three elements are in order in terms of their respective x
 * values.
 *
 * @param left The left rectangle.
 * @param center The center line.
 * @param right The right rectangle.
 * @returns true if left is right of center and center is left or right.
 */
const bool ltr(const cv::Rect left,
               const double center,
               const cv::Rect right);
//@}

/** @name Distance calculations */
//@{
/**
 * Calculates the squared distance between the centers of two rectangles.
 *
 * @param a One rectangle
 * @param b Another rectangle
 * @returns The squared distance between a and b.
 */
const double squared_center_distance(const cv::Rect a, const cv::Rect b);

/**
 * Determines which of the cv::Rect in objects is closest to the reference
 * cv::Rect.
 *
 * @param reference The reference rectangle.
 * @param objects A list of rectangles.
 * @returns the closest rectangle.
 */
const cv::Rect closest_to_center(const cv::Rect reference,
                                 const std::vector<cv::Rect> objects);

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_UTILITY_H_
