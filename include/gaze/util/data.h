// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_DATA_H_
#define INCLUDE_GAZE_UTIL_DATA_H_

#include "opencv2/core.hpp"
#include "dlib/opencv.h"
#include "dlib/image_processing.h"


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
  //@{
  /**
   * The unaltered image from the source stream.
   */
  cv::Mat source_image;

  /**
   * This image references the source_image.
   */
  dlib::array2d<dlib::bgr_pixel> image;
  //@}


  /** @name Landmarks */
  //@{
  /**
   * These landmarks are detected by the model.
   * They contain information about the face
   * (dlib::full_object_detection::get_rect()) and about the
   * eyes (use util::get_eyes_chip_details).
   */
  dlib::full_object_detection landmarks;

  /**
   * The 0th element is the left eye, the 1st the right eye.
   */
  dlib::array<dlib::array2d<dlib::bgr_pixel>> eyes;
  //@}
};

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_DATA_H_
