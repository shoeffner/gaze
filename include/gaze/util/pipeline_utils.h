#ifndef INCLUDE_GAZE_UTIL_PIPELINE_UTILS_H_
#define INCLUDE_GAZE_UTIL_PIPELINE_UTILS_H_

#include <vector>

#include "dlib/image_transforms.h"


namespace gaze {

namespace util {

/**
 * Determines a bounding box around the eyes from a given object detection.
 * This function is designed to work with 5 feature landmarks.
 *
 * @param object_detection A full object detecetion as acquired from a
 *                         dlib::shape_predictor.
 * @returns a vector of dlib::chip_details to use with
 *          dlib::extract_image_chips.
 */
std::vector<dlib::chip_details> get_eyes_chip_details(
    const dlib::full_object_detection object_detection);

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_PIPELINE_UTILS_H_
