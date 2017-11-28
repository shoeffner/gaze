#include "gaze/pipeline_step.h"

#include <string>


namespace gaze {

int PipelineStep::counter = -1;

PipelineStep::PipelineStep()
    : number(++PipelineStep::counter),
      name("Step " + std::to_string(number)) {
}

std::string PipelineStep::get_name() {
  return this->name;
}

}  // namespace gaze
