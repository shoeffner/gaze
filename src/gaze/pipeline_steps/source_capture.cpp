// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/source_capture.h"

#include <chrono>
#include <memory>
#include <string>
#include <thread>  // NOLINT

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

const void SourceCapture::init() {
  this->fps = this->video_capture->get(cv::CAP_PROP_FPS);
  this->height = this->video_capture->get(cv::CAP_PROP_FRAME_HEIGHT);
  this->width = this->video_capture->get(cv::CAP_PROP_FRAME_WIDTH);
  this->empty_frame = cv::Mat::zeros(this->height, this->width, CV_8UC3);
}

SourceCapture::SourceCapture(int source) {
  this->video_capture = std::unique_ptr<cv::VideoCapture>(
      new cv::VideoCapture(source));
  // Set goal FPS to 60, still often webcams cap much lower.
  this->video_capture->set(cv::CAP_PROP_FPS, 60.0);
  this->init();
}

SourceCapture::SourceCapture(std::string source) {
  this->video_capture = std::unique_ptr<cv::VideoCapture>(
      new cv::VideoCapture(source));
  this->init();
}

SourceCapture::~SourceCapture() {
  this->video_capture->release();
}

const cv::Mat SourceCapture::get_empty_frame() const {
  return this->empty_frame;
}

const int SourceCapture::get_frames_per_second() const {
  return this->fps;
}

const int SourceCapture::get_height() const {
  return this->height;
}

const int SourceCapture::get_width() const {
  return this->width;
}

void SourceCapture::process(util::Data* data) {
  while (!this->video_capture->read(data->source_image)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

}  // namespace pipeline

}  // namespace gaze
