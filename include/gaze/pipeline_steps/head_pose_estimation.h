// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_HEAD_POSE_ESTIMATION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_HEAD_POSE_ESTIMATION_H_

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class HeadPoseEstimation head_pose_estimation.h
 *        "gaze/pipeline_steps/head_pose_estimation.h"
 * @brief Estimates the head pose after Vatahska et al. (2007)
 *        @cite vatahska2007
 *
 * // TODO(shoeffner): Write documentation for HeadPoseEstimation.
 */
class HeadPoseEstimation : public PipelineStep {
 public:
    /**
     * Writes util::Data::head_yaw, util::Data::head_pitch, and
     * util::Data::head_roll.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    virtual void process(util::Data* data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_HEAD_POSE_ESTIMATION_H_
