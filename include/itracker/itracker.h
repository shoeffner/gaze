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

class ITrackerImpl;

class ITracker {
  ITrackerImpl* impl;
 public:
  ITracker(const std::string& model_file,
           const std::string& weights_file,
           const std::string& mean_left_eye_file,
           const std::string& mean_right_eye_file,
           const std::string& mean_face_file);

  ~ITracker();

  cv::Vec2f predict(const cv::Mat& left_eye, const cv::Mat& right_eye,
                    const cv::Mat& face, const cv::Mat& face_mask);
};

}  // namespace itracker

#endif  // INCLUDE_ITRACKER_ITRACKER_H_
