// Copyright 2017 Sebastian Höffner

#include "gaze/util/utility.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"


namespace gaze {

namespace util {

cv::CascadeClassifier load_classifier(std::string file_name) {
  // TODO(shoeffner): Currently uses hard coded path
  std::string base_path("/usr/local/share/OpenCV/haarcascades/");
  cv::CascadeClassifier classifier = cv::CascadeClassifier(
      base_path + file_name);
  if (classifier.empty()) {
    std::cerr << "[FaceDetection] Can not load cascade file " << file_name <<
      std::endl;
  }
  return classifier;
}

const bool ltr(const cv::Rect left, const double right) {
  return left.x < right;
}

const bool ltr(const cv::Rect left, const cv::Rect right) {
  return left.x < right.x;
}

const bool ltr(const double left, const cv::Rect right) {
  return left < right.x;
}

const bool ltr(const cv::Rect left,
               const double center,
               const cv::Rect right) {
  return ltr(left, center) && ltr(center, right);
}

const double squared_center_distance(const cv::Rect a, const cv::Rect b) {
  double ax = (a.x + a.width / 2);
  double ay = (a.y + a.height / 2);
  double bx = (a.x + a.width / 2);
  double by = (a.y + a.height / 2);
  return (ax - bx) * (ax - bx) + (ay - by) * (ay - by);
}

const cv::Rect closest_to_center(const cv::Rect reference,
                                 const std::vector<cv::Rect> objects) {
  cv::Rect closest;
  double closest_distance = std::numeric_limits<double>::max();
  for (cv::Rect r : objects) {
    double d = squared_center_distance(reference, r);
    if (d < closest_distance) {
      closest_distance = d;
      closest = r;
    }
  }
  return closest;
}

}  // namespace util

}  // namespace gaze
