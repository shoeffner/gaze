// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/head_pose_estimation.h"

#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

void HeadPoseEstimation::process(util::Data* data) {
  // TODO(shoeffner): Implement head pose estimation.
  data->head_yaw = 0;
  data->head_pitch = 0;
  data->head_roll = 0;
}

}  // namespace pipeline

}  // namespace gaze
