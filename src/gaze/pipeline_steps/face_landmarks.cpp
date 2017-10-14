// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/face_landmarks.h"

#include <vector>

#include "dlib/image_processing.h"
#include "dlib/image_processing/frontal_face_detector.h"
#include "opencv2/imgproc.hpp"

#include "gaze/util/data.h"
#include "gaze/util/utility.h"


namespace gaze {

namespace pipeline {

FaceLandmarks::FaceLandmarks()
    : face_detector(dlib::get_frontal_face_detector()) {
  // TODO(shoeffner): Install 5 face landmarks model using CMake.
  dlib::deserialize("shape_predictor_5_face_landmarks.dat")
    >> this->shape_predictor;
}

void FaceLandmarks::process(util::Data& data) {
  std::vector<dlib::rectangle> faces = this->face_detector(data.image);
  if (faces.size() > 0) {
    data.landmarks = this->shape_predictor(data.image, faces[0]);
  }
}

void FaceLandmarks::visualize(util::Data& data) {
  // Bounding box face
  if (data.landmarks.get_rect().is_empty()) {
    return;
  }
  cv::rectangle(data.source_image,
                util::crop_to_image_boundary(
                  data.source_image,
                  util::convert(data.landmarks.get_rect())),
                cv::Scalar(255, 0, 0));

  // landmark lines (similar to dlib::render_face_detections
  if (data.landmarks.num_parts() != 5) {
    return;
  }
  std::vector<cv::Point> points;
  for (int i : {0, 1, 4, 3, 2}) {
    points.push_back(util::convert(data.landmarks.part(i)));
  }
  cv::polylines(data.source_image, points,
                false, cv::Scalar(0, 255, 125), 2);
}

}  // namespace pipeline

}  // namespace gaze
