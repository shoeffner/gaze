// Copyright 2017 Sebastian Höffner

#include "gaze/util/utility.h"

#include <iostream>
#include <string>

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

}  // namespace util

}  // namespace gaze
