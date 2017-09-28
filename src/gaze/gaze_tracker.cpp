// Copyright 2017 Sebastian Höffner

#include "gaze/gaze.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include "gaze/source_capture.h"
#include "gaze/util/spmc_deque.h"


namespace gaze {

GazeTracker::GazeTracker() {
  this->initialized = false;
}

GazeTracker::GazeTracker(const int source,
                         const std::string subject_id,
                         const std::string result_dir) {
  this->init(source, subject_id, result_dir);
}

GazeTracker::GazeTracker(const std::string source,
                         const std::string subject_id,
                         const std::string result_dir) {
  this->init(source, subject_id, result_dir);
}

GazeTracker::~GazeTracker() {
  if (this->initialized) {
    this->source_capture_thread->join();
    delete this->source_capture_thread;
    delete this->source_capture;
    delete this->source_image_queue;
  }
}

const void GazeTracker::calibrate() {
  std::cout << "[GT Stub] Calibrating." << std::endl;
}

const cv::Mat GazeTracker::get_current_frame() const {
  return this->source_image_queue->back_or_default(
                this->source_capture->get_empty_frame());
}

const std::pair<int, int> GazeTracker::get_current_gaze_point() const {
  // TODO(shoeffner): Remove dummy values once tracking is implemented.
  return std::pair<int, int>(120, 130);
}

const void GazeTracker::init(const int source,
                             const std::string subject_id,
                             const std::string result_dir) {
  this->init(std::to_string(source), subject_id, result_dir);
}

const void GazeTracker::init(const std::string source,
                             const std::string subject_id,
                             const std::string result_dir) {
  try {
    int cam_source = std::stoi(source);
    this->source_type = SourceType::WEBCAM;
    this->source_capture = new SourceCapture(cam_source);
  } catch (const std::invalid_argument&) {
    this->source_type = SourceType::VIDEO;
    this->source_capture = new SourceCapture(source);
  }
  this->source_image_queue = new util::SPMCDeque<cv::Mat>();
  this->source_capture_thread = new std::thread(
      std::ref(*(this->source_capture)),
      std::ref(this->source_image_queue));
  this->result_dir = result_dir;
  this->subject_id = subject_id;
  this->initialized = true;
}

const void GazeTracker::print_capture_info() const {
  if (!this->initialized) {
    std::cerr << "[GazeTracker] is not initialized." << std::endl;
    return;
  }

  // TODO(shoeffner): Add more information about video sources
  if (source_type == SourceType::WEBCAM) {
    std::cout << "[GazeTracker] source is webcam " << this->video_source
      << std::endl;
  } else {
    std::cout << "[GazeTracker] source is video file " << this->video_source
      << std::endl;
  }
}

const void GazeTracker::print_info() const {
  this->print_capture_info();
  std::cout << "[GazeTracker] Subject ID: " << this->subject_id << std::endl;
  std::cout << "[GazeTracker] Result directory: " << this->result_dir
    << std::endl;
}

const void GazeTracker::show_debug_screen() const {
  cv::Mat image = this->source_capture->get_empty_frame();
  int key = -1;
  while (true) {
    image = this->source_image_queue->back_or_default(image);
    cv::imshow("GazeTracker Debug", image);
    key = cv::waitKey(1);
    if (key != -1) {
      break;
    }
  }
}

const void GazeTracker::start_trial(const std::string identifier) {
  this->current_trial_id = identifier;
  std::cout << "[GT Stub] Starting trial " << this->current_trial_id
    << std::endl;
}

const void GazeTracker::stop_trial() {
  std::cout << "[GT Stub] Stopping trial " << this->current_trial_id
    << std::endl;
  this->current_trial_id = "";
}

}  // namespace gaze
