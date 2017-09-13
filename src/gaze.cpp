// Copyright 2017 Sebastian Höffner

#include "gaze/gaze.h"

#include <iostream>
#include <memory>
#include <string>

#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"


namespace gaze {

GazeTracker::GazeTracker(std::string source) {
  video_source = source;
  // Try to parse source as an integer to select webcam device.
  // Otherwise assume filepath and open video file.
  try {
    video_capture = std::unique_ptr<cv::VideoCapture>(
        new cv::VideoCapture(std::stoi(video_source)));
    // Set goal FPS to 60, still often webcams cap much lower.
    video_capture->set(cv::CAP_PROP_FPS, 60.0);
    source_type = SourceType::WEBCAM;
  } catch (const std::invalid_argument& no_int_ignoring_exception) {
    video_capture = std::unique_ptr<cv::VideoCapture>(
        new cv::VideoCapture(video_source));
    source_type = SourceType::VIDEO;
  }
}

GazeTracker::~GazeTracker() {
  video_capture->release();
}

void GazeTracker::print_capture_info() {
  if (source_type == SourceType::WEBCAM) {
    std::cout << "GazeTracker source is webcam " << video_source
      << ", w x h: " << video_capture->get(cv::CAP_PROP_FRAME_WIDTH)
      << "x" << video_capture->get(cv::CAP_PROP_FRAME_HEIGHT)
      << ", FPS: " << video_capture->get(cv::CAP_PROP_FPS)
      << std::endl;
  } else {
    // TODO(shoeffner): Add more information for video files
    std::cout << "GazeTracker source is video file " <<  video_source <<
      std::endl;
  }
}

void GazeTracker::show_debug_screen() {
  cv::Mat image;
  *video_capture >> image;
  cv::imshow("gaze debug screen", image);
  cv::waitKey(1);
}


}  // namespace gaze
