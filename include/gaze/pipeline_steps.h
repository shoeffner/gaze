// Copyright 2017 Sebastian Höffner
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
#include "gaze/pipeline_steps/debug_view.h"
#include "gaze/pipeline_steps/head_pose_estimation.h"
#include "gaze/pipeline_steps/into_the_void_step.h"
#include "gaze/pipeline_steps/source_capture.h"

#include "gaze/util/data.h"
#include "gaze/util/spsc_deque.h"

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_H_
