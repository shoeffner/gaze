// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/face_landmarks.h"

#include <vector>

#include "dlib/image_processing.h"
#include "dlib/image_processing/frontal_face_detector.h"
#include "opencv2/imgproc.hpp"

#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

FaceLandmarks::FaceLandmarks()
    : face_detector(dlib::get_frontal_face_detector()) {
  // TODO(shoeffner): Install 5 face landmarks model using CMake.
  this->name = "FaceLandmarks";
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
    this->widget->set_image(data.image);
    return;
  }
  dlib::array2d<dlib::bgr_pixel> face_image;
  dlib::assign_image(face_image, data.image);
  dlib::draw_rectangle(face_image,
                       data.landmarks.get_rect(),
                       dlib::bgr_pixel(255, 0, 0));

  // landmark lines (similar to dlib::render_face_detections
  if (data.landmarks.num_parts() != 5) {
    this->widget->set_image(face_image);
    return;
  }

  dlib::point* start = &data.landmarks.part(0);
  for (int i : {1, 4, 3, 2}) {
    dlib::point* end = &data.landmarks.part(i);
    dlib::draw_line(face_image,
                    *start,
                    *end,
                    dlib::bgr_pixel(0, 255, 125));
    start = end;
  }
  this->widget->set_image(face_image);
}

}  // namespace pipeline

}  // namespace gaze
