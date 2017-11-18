// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/gaze_point_calculation.h"

#include <string>

#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

GazePointCalculation::GazePointCalculation() {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "GazePoint";
}

void GazePointCalculation::process(util::Data& data) {
}

void GazePointCalculation::visualize(util::Data& data) {
}

}  // namespace pipeline

}  // namespace gaze
