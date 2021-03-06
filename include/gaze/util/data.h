#ifndef INCLUDE_GAZE_UTIL_DATA_H_
#define INCLUDE_GAZE_UTIL_DATA_H_

#include <chrono>  // NOLINT
#include <iostream>
#include <map>
#include <string>
#include <utility>  // Not used, but cpplint detects swap() as part of this
#include <vector>

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
  /** @name Constructors */
  //@{
  Data();
  Data(const Data& data);

  /**
   * Uses copy and Data::swap().
   *
   * @param data the data object to assign
   * @returns a reference to `this`.
   */
  Data& operator=(Data data);
  //@}

  /** @name Input image */
  //@{
  /**
   * The unaltered image from the source stream.
   */
  cv::Mat source_image;

  /**
   * The source image for dlib.
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
   * The 0th element is the right eye, the 1st the left eye.
   */
  dlib::array<dlib::array2d<double>> eyes;
  //@}

  /**
   * Eye center locations.
   */
  dlib::array<dlib::point> centers;

  /** @name Head pose */
  //@{
  /**
   * The head orientation.
   */
  cv::Mat head_rotation;

  /**
   * The head translation.
   */
  cv::Mat head_translation;
  //@}

  /** @name Gaze point */
  //@{
  /**
   * The pupils in model coordinates.
   */
  std::vector<cv::Vec3d> pupils;

  /**
   * The gaze points in model coordinates.
   */
  std::vector<cv::Vec3d> gaze_points;

  /**
   * The mean gaze point in screen coordinates.
   */
  cv::Vec2d estimated_gaze_point;
  //@}

  /**
   * Maps the step identifiers to their processing times.
   */
  std::map<std::string, double> execution_times;

 private:
  /**
   * Swaps `this`'s members with `data`'s.
   *
   * @param data the data object to swap with.
   */
  void swap(Data& data);
};

/**
 * Prints a human readable short information about the data object to the
 * stream.
 *
 * @param ostr the stream.
 * @param data the data object.
 * @returns the stream.
 */
inline std::ostream& operator<<(std::ostream& ostr,
                                const Data& data) {
  ostr << "Source image size: " << data.source_image.size() << std::endl
       << "dlib image size: " << data.image.size() << std::endl
       << "Face: " << data.landmarks.get_rect() << std::endl
       << "Eyes detected: " << data.eyes.size() << std::endl;
  return ostr;
}

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_DATA_H_
