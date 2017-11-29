#ifndef INCLUDE_GAZE_PIPELINE_STEPS_FALLBACK_STEP_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_FALLBACK_STEP_H_

#include <string>

#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class FallbackStep fallback_step.h "gaze/pipeline_steps/fallback_step.h"
 * @brief This pipeline step is used whenever something goes wrong.
 */
class FallbackStep final
    : public PipelineStep,
      public gui::LabelVisualizeable {
  std::string config;

 public:
  /**
   * Initializes the FallbackStep.
   * Reads all configuration for this step, in case something went wrong.
   */
  FallbackStep();

  /**
   * Does nothing.
   */
  void process(util::Data&) override;

  /**
   * Shows instructions on how to implement a custom pipeline step and register
   * it successfully.
   * Shows all information from gaze.yaml for this step (even if its the wrong
   * step).
   */
  void visualize(util::Data&) override;
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_FALLBACK_STEP_H_
