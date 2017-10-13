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

FaceLandmarks::FaceLandmarks() {
  this->face_detector = dlib::get_frontal_face_detector();
  // TODO(shoeffner): Install 5 face landmarks model using CMake.
  dlib::deserialize("shape_predictor_5_face_landmarks.dat")
    >> this->shape_predictor;
}

void FaceLandmarks::process(util::Data* data) {
  std::vector<dlib::rectangle> faces = this->face_detector(data->image);
  if (faces.size() > 0) {
    data->landmarks = this->shape_predictor(data->image, faces[0]);
  }
}

void FaceLandmarks::visualize(util::Data* data) {
  cv::rectangle(data->source_image,
                util::convert(data->landmarks.get_rect()),
                cv::Scalar(255, 0, 0));
}

}  // namespace pipeline

}  // namespace gaze
