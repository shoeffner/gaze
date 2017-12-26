#ifndef INCLUDE_GAZE_PIPELINE_STEPS_EYE_LIKE_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_EYE_LIKE_H_

#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class EyeLike eye_like.h "gaze/pipeline_steps/eye_like.h"
 * @brief Direct copy of https://github.com/trishume/eyelike
 */
class EyeLike final
    : public PipelineStep,
      public gui::ImageVisualizeable {
  double relative_threshold_factor;
  double sigma_factor;

 public:
  /**
   */
  EyeLike();

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

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_EYE_LIKE_H_
