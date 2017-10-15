// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_UTILITY_H_
#define INCLUDE_GAZE_UTIL_UTILITY_H_

#include <vector>

#include "dlib/image_processing.h"
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
 * Converts between Rect and rectangle.
 *
 * @param in Rect to convert.
 * @returns converted rectangle.
 */
const dlib::rectangle convert(const cv::Rect in);

/**
 * Converts between Rect and rectangle.
 *
 * @param in rectangle to convert.
 * @returns converted Rect.
 */
const cv::Rect convert(const dlib::rectangle in);

/**
 * Converts between Point and point.
 *
 * @param in the Point to convert.
 * @returns converted point.
 */
const dlib::point convert(const cv::Point in);

/**
 * Converts between Point and point.
 *
 * @param in the point to convert.
 * @returns converted Point.
 */
const cv::Point convert(const dlib::point in);
//@}

/**
 * Determines a bounding box around the eyes from a given object detection.
 * This function is designed to work with 5 feature landmarks.
 * // TODO(shoeffner): Add support for the 68 features.
 *
 * @param object_detection A full object detecetion as acquired from a
 *                         dlib::shape_predictor.
 * @returns a vector of dlib::chip_details to use with
 *          dlib::extract_image_chips.
 */
std::vector<dlib::chip_details> get_eyes_chip_details(
    const dlib::full_object_detection object_detection);

/** @name Debug drawing helpers */
//@{
/**
 * Crops the region of interest to avoid overlaps of the image (and not cause
 * OpenCV to crash). Found here:
 * http://answers.opencv.org/question/70953/roi-out-of-bounds-issue/?answer=70960
 *
 * @param image the image to fit the region of interest to.
 * @param roi the region of interest.
 * @returns a cropped region of interest.
 */
const cv::Rect crop_to_image_boundary(const cv::Mat& image,
                                      const cv::Rect& roi);

/**
 * Checks if a point lies inside the image.
 *
 * @param image The image.
 * @param poi The point of interest.
 * @returns true if the point is inside the image
 */
bool in_image(const cv::Mat& image, const cv::Point& poi);
//@}

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_UTILITY_H_
