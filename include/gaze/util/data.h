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
  double head_yaw = 0;
  // The head's pitch angle.
  double head_pitch = 0;
  // The head's roll angle.
  double head_roll = 0;
  //@}

  /** @name Face detection
   *
   * These parameters denote where a head was detected.
   */
  //@{
  bool face_found = false;
  bool frontal_face_found = false;
  cv::Rect face;
  //@}

  /** @name Eye detection
   *
   * These parameters denote where each eye was detected.
   */
  //@{
  bool left_eye_found = false;
  cv::Rect left_eye;
  bool right_eye_found = false;
  cv::Rect right_eye;
  //@}
};

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_DATA_H_
