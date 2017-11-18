// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/face_landmarks.h"

#include <string>
#include <vector>

#include "dlib/image_processing.h"
#include "dlib/image_processing/frontal_face_detector.h"
#include "dlib/image_processing/render_face_detections.h"
#include "opencv2/imgproc.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

FaceLandmarks::FaceLandmarks()
    : face_detector(dlib::get_frontal_face_detector()) {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "FaceLandmarks";

  std::string landmarks_model("shape_predictor_5_face_landmarks.dat");
  dlib::deserialize(config["model"] ?
                    config["model"].as<std::string>() :
                    landmarks_model) >> this->shape_predictor;
}

void FaceLandmarks::process(util::Data& data) {
  std::vector<dlib::rectangle> faces = this->face_detector(data.image);
  if (faces.size() > 0) {
    data.landmarks = this->shape_predictor(data.image, faces[0]);
  }
}

void FaceLandmarks::visualize(util::Data& data) {
  this->widget->clear_overlay();

  // Reprocess the data in case multiple pipeline steps
  // interfered - this is good for comparisons.
  this->process(data);

  if (data.landmarks.get_rect().is_empty()) {
    this->widget->set_image(data.image);
    return;
  }

  this->widget->set_image(data.image);
  this->widget->add_overlay(dlib::image_display::overlay_rect(
        data.landmarks.get_rect(),
        dlib::rgb_pixel(0, 0, 255)));
  this->widget->add_overlay(dlib::render_face_detections(data.landmarks));
}

}  // namespace pipeline

}  // namespace gaze
