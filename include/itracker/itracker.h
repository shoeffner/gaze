/**
 * @namespace itracker
 * @brief The itracker library provides a wrapper around the caffe model.
 *
 * TODO: Add more information
 */

#ifndef INCLUDE_ITRACKER_ITRACKER_H_
#define INCLUDE_ITRACKER_ITRACKER_H_

#include <string>
#include <vector>

#include "opencv2/opencv.hpp"


namespace itracker {

/**
 * @class ITrackerImpl itracker.h "itracker/itracker.h"
 * @brief A private implementation of ITracker to hide dependencies from including classes.
 *
 * This is only a forward declaration.
 */
class ITrackerImpl;

/**
 * @class ITracker itracker.h "itracker/itracker.h"
 * @brief Implements a wrapper around the pre-trained caffe model by @cite Krafka2016.
 */
class ITracker {
  ITrackerImpl* impl;

 public:
  /**
   * Initializes the iTracker.
   *
   * @param model_file A path to the model file, usually a *_deploy.prototxt
   * @param weights_file A path to the trained weights, usually a *.caffemodel
   * @param mean_left_file: Path to a *.binaryproto file, holds the mean of
   *                        left eyes of the training set
   * @param mean_right_file: Path to a *.binaryproto file, holds the mean of
   *                         right eyes of the training set
   * @param mean_face_file: Path to a *.binaryprotofile, holds the mean of
   *                        faces of the training set
   */
  ITracker(const std::string& model_file,
           const std::string& weights_file,
           const std::string& mean_left_eye_file,
           const std::string& mean_right_eye_file,
           const std::string& mean_face_file);

  ~ITracker();

  /**
   * Performs a forward step for the neural network.
   *
   * @param left_eye A cropped left eye.
   * @param right_eye A cropped right eye.
   * @param face The cropped face
   * @param face_mask A 25x25 matrix which has 1 where the face was in the
   *                  original image (relative), and 0 at the other places
   *
   * @returns the prediction, x and y offsets from the camera in cm.
   */
  cv::Vec2f predict(const cv::Mat& left_eye, const cv::Mat& right_eye,
                    const cv::Mat& face, const cv::Mat& face_mask);
};

}  // namespace itracker

#endif  // INCLUDE_ITRACKER_ITRACKER_H_
