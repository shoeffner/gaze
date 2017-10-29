// Copyright 2017 Sebastian Höffner

#include "gaze/gaze_tracker.h"

#include <iostream>
#include <string>
#include <utility>

#include "opencv2/core.hpp"

#include "gaze/gui/debug_window.h"
#include "gaze/pipeline.h"
#include "gaze/pipeline_step.h"
#include "gaze/pipeline_steps.h"


namespace gaze {

GazeTracker::GazeTracker(const bool debug) : debug(debug) {
  this->initialized = false;
}

GazeTracker::GazeTracker(const int source,
                         const std::string subject_id,
                         const std::string result_dir,
                         const bool debug)
    : debug(debug),
      result_dir(result_dir),
      subject_id(subject_id) {
  this->initialized = false;
  this->init(source, subject_id, result_dir, debug);
}

GazeTracker::GazeTracker(const std::string source,
                         const std::string subject_id,
                         const std::string result_dir,
                         const bool debug)
    : debug(debug),
      result_dir(result_dir),
      subject_id(subject_id) {
  this->initialized = false;
  this->init(source, subject_id, result_dir, debug);
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

void GazeTracker::init(const int source,
                       const std::string subject_id,
                       const std::string result_dir,
                       const bool debug) {
  this->init(std::to_string(source), subject_id, result_dir, debug);
}

void GazeTracker::init(const std::string source,
                       const std::string subject_id,
                       const std::string result_dir,
                       const bool debug) {
  if (this->initialized) {
    return;
  }
  this->debug = debug;
  try {
    int cam_source = std::stoi(source);
    this->pipeline_steps.push_back(new pipeline::SourceCapture(cam_source));
  } catch (const std::invalid_argument&) {
    this->pipeline_steps.push_back(new pipeline::SourceCapture(source));
  }
  this->result_dir = result_dir;
  this->subject_id = subject_id;
  this->init_pipeline();
  if (this->debug) {
    gui::open_debug_window(this->pipeline);
  }
  this->initialized = true;
}

void GazeTracker::init_pipeline() {
  if (this->initialized) {
    return;
  }
  this->pipeline_steps.push_back(new pipeline::FaceLandmarks());
  this->pipeline_steps.push_back(new pipeline::HeadPoseEstimation());
  this->pipeline_steps.push_back(new pipeline::PupilLocalization());
  this->pipeline_steps.push_back(new pipeline::GazePointCalculation());
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
