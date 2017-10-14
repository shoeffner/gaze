// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_step.h"

#include <string>

#include "opencv2/imgproc.hpp"

namespace gaze {

int PipelineStep::counter = 0;

PipelineStep::PipelineStep() {
  this->number = PipelineStep::counter++;
}

void PipelineStep::visualize(util::Data&) {
  // Do nothing by default.
}

}  // namespace gaze
