// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_UTILITY_H_
#define INCLUDE_GAZE_UTIL_UTILITY_H_

#include <string>

#include "opencv2/objdetect.hpp"


namespace gaze {

namespace util {

/**
 * Helper function to load classifiers by path.
 * Prints to STDERR if a classifier can not be loaded.
 *
 * @param file_name The cascade filename to load the xml from.
 * @returns the CascadeClassifier, might not be loaded.
 */
cv::CascadeClassifier load_classifier(std::string file_name);

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_UTILITY_H_
