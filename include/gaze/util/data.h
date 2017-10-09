// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_DATA_H_
#define INCLUDE_GAZE_UTIL_DATA_H_

#include "opencv2/core.hpp"


namespace gaze {

namespace util {

/**
 * @struct Data data.h "gaze/util/data.h"
 * @brief Wraps the data acquired per frame into a single instance.
 *
 * This data can then be written in different data formats.
 * // TODO(shoeffner): Write at least one writer.
 */
struct Data {
  /**
   * The unaltered image from the source stream.
   */
  cv::Mat raw_capture_image;

  /**
   * Constructs a new data item with an initial image from the source stream.
   *
   * @param raw_capture_image The image from the source stream.
   */
  Data(const cv::Mat raw_capture_image);

};

/**
 * A Placeholder with dummy data for visualizations.
 */
static Data DATA_PLACEHOLDER = Data(cv::Mat::zeros(1, 1, CV_8UC3));

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_DATA_H_
