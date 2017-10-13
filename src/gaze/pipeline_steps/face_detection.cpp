// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/face_detection.h"

#include <string>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include "gaze/util/data.h"
#include "gaze/util/utility.h"


namespace gaze {

namespace pipeline {

FaceDetection::FaceDetection() {
  std::string frontal_path("haarcascade_frontalface_default.xml");
  std::string profile_path("haarcascade_profileface.xml");
  this->frontal_face_classifier = util::load_classifier(frontal_path);
  this->profile_face_classifier = util::load_classifier(profile_path);
}

void FaceDetection::process(util::Data* data) {
  std::vector<cv::Rect> detections;
  this->frontal_face_classifier.detectMultiScale(data->source_image,
                                                 detections);
  if (detections.empty()) {
    this->profile_face_classifier.detectMultiScale(data->source_image,
                                                   detections);
  }
  if (!detections.empty()) {
    data->face_found = true;
    data->face = detections[0];
  } else {
    data->face_found = false;
  }
}

void FaceDetection::visualize(util::Data* data) {
  if (data->face_found) {
    cv::rectangle(data->source_image, data->face, cv::Scalar(255, 0, 0));
    this->write_text(&data->source_image, "Face detection: Face detected.");
  } else {
    this->write_text(&data->source_image, "Face detection: No face detected.");
  }
}

}  // namespace pipeline

}  // namespace gaze
