// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/head_pose_estimation.h"

#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

HeadPoseEstimation::HeadPoseEstimation() {
  this->name = "HeadPose";
}

void HeadPoseEstimation::process(util::Data& data) {
}

void HeadPoseEstimation::visualize(util::Data& data) {
}

}  // namespace pipeline

}  // namespace gaze
