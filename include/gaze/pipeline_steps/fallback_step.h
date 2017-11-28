#ifndef INCLUDE_GAZE_PIPELINE_STEPS_FALLBACK_STEP_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_FALLBACK_STEP_H_

#include <string>

#include "dlib/gui_widgets.h"

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
   */
  FallbackStep();

  /**
   * Detects the eye center for each eye.
   *
   * @param data The data object to be updated.
   * @returns via `data` the modified data object.
   */
  void process(util::Data& data) override;

  /**
   * @param data The data object to be updated.
   */
  void visualize(util::Data& data) override;
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_FALLBACK_STEP_H_
