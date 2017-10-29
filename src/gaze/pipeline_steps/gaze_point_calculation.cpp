// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/gaze_point_calculation.h"

#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

GazePointCalculation::GazePointCalculation() {
  this->name = "GazePoint";
}

void GazePointCalculation::process(util::Data& data) {
}

void GazePointCalculation::visualize(util::Data& data) {
}

}  // namespace pipeline

}  // namespace gaze
