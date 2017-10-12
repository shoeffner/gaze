// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/face_detection.h"

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * Helper function to load classifiers by path.
 * Prints to STDERR if a classifier can not be loaded.
 *
 * @param path The path to load the xml from.
 * @returns the CascadeClassifier, might not be loaded.
 */
cv::CascadeClassifier load_classifier(std::string path) {
  cv::CascadeClassifier classifier = cv::CascadeClassifier(path);
  if (classifier.empty()) {
    std::cerr << "[FaceDetection] Can not load cascade file " << path <<
      std::endl;
  }
  return classifier;
}

FaceDetection::FaceDetection() {
  // TODO(shoeffner): haarcascade file location needs to be independent.
  std::string base_path("/usr/local/share/OpenCV/haarcascades/");
  std::string frontal_path("haarcascade_frontalface_default.xml");
  std::string profile_path("haarcascade_profileface.xml");
  this->frontal_face_classifier = load_classifier(base_path + frontal_path);
  this->profile_face_classifier = load_classifier(base_path + profile_path);
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
