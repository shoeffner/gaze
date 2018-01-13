/**
 * @namespace gaze::pipeline
 * @brief This namespace contains pipeline steps.
 *
 * All pipeline steps are bundled inside this namespace to make them easily
 * accesible.
 */

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_H_

#include "gaze/pipeline_step.h"
#include "gaze/pipeline_steps/eye_like.h"
#include "gaze/pipeline_steps/face_landmarks.h"
#include "gaze/pipeline_steps/fallback_step.h"
#include "gaze/pipeline_steps/gaze_capture.h"
#include "gaze/pipeline_steps/gaze_point_calculation.h"
#include "gaze/pipeline_steps/head_pose_estimation.h"
#include "gaze/pipeline_steps/pupil_localization.h"
#include "gaze/pipeline_steps/source_capture.h"

#include "gaze/util/data.h"

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_H_
