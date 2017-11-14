// Copyright 2017 Sebastian Höffner

#include "gaze/gaze_tracker.h"

#include <iostream>
#include <string>
#include <utility>

#include "opencv2/core.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/gui/debug_window.h"
#include "gaze/pipeline.h"
#include "gaze/pipeline_step.h"
#include "gaze/pipeline_steps.h"
#include "gaze/util/config.h"


namespace gaze {

GazeTracker::GazeTracker(const std::string subject_id,
                         bool debug)
    : debug(debug),
      subject_id(subject_id) {
  this->initialized = false;
  this->init(subject_id, debug);
}

GazeTracker::~GazeTracker() {
  if (this->initialized) {
    delete this->pipeline;
    for (PipelineStep* step : this->pipeline_steps) {
      delete step;
    }
  }
}

void GazeTracker::calibrate() {
  if (!this->initialized) {
    std::cerr << "[GazeTracker] Not initialized (calibrate())." << std::endl;
    return;
  }
  std::cout << "[GT Stub] Calibrating." << std::endl;
}

const std::pair<int, int> GazeTracker::get_current_gaze_point() const {
  if (!this->initialized) {
    std::cerr << "[GazeTracker] Not initialized (get_current_gaze_point())." <<
      std::endl;
    return std::pair<int, int>(-1, -1);
  }
  // TODO(shoeffner): Remove dummy values once tracking is implemented.
  return std::pair<int, int>(120, 130);
}

void GazeTracker::init(const std::string subject_id,
                       const bool debug) {
  if (this->initialized) {
    return;
  }
  this->debug = debug;
  this->subject_id = subject_id;
  this->init_pipeline(subject_id);
  if (this->debug) {
    gui::open_debug_window(this->pipeline);
  }
  this->initialized = true;
}

void GazeTracker::init_pipeline(const std::string subject_id) {
  if (this->initialized) {
    return;
  }
  YAML::Node config = util::get_config()["pipeline"];
  std::cout << config << std::endl;
  for (YAML::Node step_config : config) {
    std::string type  = step_config["type"].as<std::string>();
    PipelineStep* step;
    if (!type.compare("FaceLandmarks")) {
      step = new pipeline::FaceLandmarks();
    } else if (!type.compare("GazePointCalculation")) {
      step = new pipeline::GazePointCalculation();
    } else if (!type.compare("HeadPoseEstimation")) {
      step = new pipeline::HeadPoseEstimation();
    } else if (!type.compare("PupilLocalization")) {
      step = new pipeline::PupilLocalization();
    } else if (!type.compare("SourceCapture")) {
      step = new pipeline::SourceCapture();
    }
    this->pipeline_steps.push_back(step);
  }
  this->pipeline = new Pipeline(this->pipeline_steps, true);
}

void GazeTracker::start_trial(const std::string identifier) {
  if (!this->initialized) {
    std::cerr << "[GazeTracker] Not initialized (start_trial())." <<
      std::endl;
    return;
  }
  this->current_trial_id = identifier;
  std::cout << "[GazeTracker] [Stub] Starting trial " << this->current_trial_id
    << std::endl;
}

void GazeTracker::stop_trial() {
  if (!this->initialized) {
    std::cerr << "[GazeTracker] Not initialized (stop_trial())." <<
      std::endl;
    return;
  }
  std::cout << "[GazeTracker] [Stub] Stopping trial " << this->current_trial_id
    << std::endl;
  this->current_trial_id = "";
}

}  // namespace gaze
