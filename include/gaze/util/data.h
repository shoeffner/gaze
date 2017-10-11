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
  cv::Mat source_image;

  /** @name Head pose estimates
   *
   * These head pose parameters are estimated according to
   * Vatahska et al. (2007) @cite vatahska2007.
   */
  //@{
  // The head's yaw angle.
  double head_yaw;
  // The head's pitch angle.
  double head_pitch;
  // The head's roll angle.
  double head_roll;
  //@}


};

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_DATA_H_
