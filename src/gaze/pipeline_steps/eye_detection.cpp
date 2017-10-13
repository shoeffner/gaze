// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/eye_detection.h"

#include <string>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include "gaze/util/data.h"
#include "gaze/util/utility.h"


namespace gaze {

namespace pipeline {

EyeDetection::EyeDetection() {
  std::string eye_path("haarcascade_frontalface_default.xml");
  this->eye_classifier = util::load_classifier(eye_path);
}

void EyeDetection::process(util::Data* data) {
  if (!data->face_found) {
    return;
  }
  std::vector<cv::Rect> detections;
  this->eye_classifier.detectMultiScale(
      data->source_image(data->face), detections);
  if (!detections.empty()) {
    // TODO(shoeffner): Determine which eye(s) have been found and assign
    // properly
    data->left_eye_found = true;
    data->left_eye = detections[0];
    if (detections.size() > 1) {
      data->right_eye_found = true;
      data->right_eye = detections[1];
    }
  }
}

void EyeDetection::visualize(util::Data* data) {
  int eyes = data->left_eye_found + data->right_eye_found;
  if (eyes) {
    if (data->left_eye_found) {
      cv::rectangle(data->source_image,
                    data->left_eye,
                    cv::Scalar(0, 180, 0));
    }
    if (data->right_eye_found) {
      cv::rectangle(data->source_image,
                    data->right_eye,
                    cv::Scalar(0, 255, 0));
    }
    this->write_text(&data->source_image,
        "Eye detection: " + std::to_string(eyes) + " detected.");
  } else {
    this->write_text(&data->source_image, "Eye detection: None detected.");
  }
}

}  // namespace pipeline

}  // namespace gaze
