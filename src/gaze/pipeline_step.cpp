// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_step.h"

#include <memory>
#include <string>

#include "opencv2/imgproc.hpp"

#include "gaze/gui/pipeline_step_widget.h"


namespace gaze {

int PipelineStep::counter = -1;

PipelineStep::PipelineStep()
    : number(++PipelineStep::counter),
      name("Step " + std::to_string(number)) {
}

void PipelineStep::visualize(util::Data&) {
  // Do nothing by default.
}

std::string PipelineStep::get_name() {
  return this->name;
}

void PipelineStep::set_widget(std::shared_ptr<gui::PipelineStepWidget> widget) {
  this->widget = widget;
}

}  // namespace gaze
