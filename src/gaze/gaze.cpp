// Copyright 2017 Sebastian Höffner

#include "gaze/gaze.h"

#include <iostream>
#include <memory>
#include <string>

#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"


namespace gaze {

GazeTracker::GazeTracker() {
  this->initialized = false;
}

GazeTracker::GazeTracker(int source) {
  this->init(source);
}

GazeTracker::GazeTracker(std::string source) {
  this->init(source);
}

const void GazeTracker::init(int source) {
  this->video_source = std::to_string(source);
  this->video_capture = std::unique_ptr<cv::VideoCapture>(
        new cv::VideoCapture(source));
  // Set goal FPS to 60, still often webcams cap much lower.
  this->video_capture->set(cv::CAP_PROP_FPS, 60.0);
  this->source_type = SourceType::WEBCAM;
  this->initialized = true;
}

const void GazeTracker::init(std::string source) {
  this->video_source = source;
  // Try to parse source as an integer to select webcam device.
  // Otherwise assume filepath and open video file.
  try {
    this->init(std::stoi(video_source));
  } catch (const std::invalid_argument&) {
    this->video_capture = std::unique_ptr<cv::VideoCapture>(
        new cv::VideoCapture(video_source));
    this->source_type = SourceType::VIDEO;
  }
  this->initialized = true;
}

GazeTracker::~GazeTracker() {
  if (this->initialized) {
    this->video_capture->release();
  }
}

const void GazeTracker::print_capture_info() const {
  if (!this->initialized) {
    std::cerr << "GazeTracker is not initialized." << std::endl;
    return;
  }

  if (source_type == SourceType::WEBCAM) {
    std::cout << "GazeTracker source is webcam " << this->video_source
      << ", w x h: " << this->video_capture->get(cv::CAP_PROP_FRAME_WIDTH)
      << "x" << this->video_capture->get(cv::CAP_PROP_FRAME_HEIGHT)
      << ", FPS: " << this->video_capture->get(cv::CAP_PROP_FPS)
      << std::endl;
  } else {
    // TODO(shoeffner): Add more information for video files
    std::cout << "GazeTracker source is video file " << this->video_source <<
      std::endl;
  }
}

const void GazeTracker::show_debug_screen() const {
  cv::Mat image;
  *(this->video_capture) >> image;
  cv::imshow("gaze debug screen", image);
  cv::waitKey(1);
}

}  // namespace gaze
