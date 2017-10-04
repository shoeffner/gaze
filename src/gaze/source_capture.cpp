// Copyright 2017 Sebastian Höffner

#include "gaze/source_capture.h"

#include <chrono>  // NOLINT
#include <memory>
#include <string>
#include <thread>  // NOLINT

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include "gaze/util/spsc_deque.h"


namespace gaze {

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

const void SourceCapture::operator()(
    util::SPSCDeque<cv::Mat>* const share_deque)
  const {
  cv::Mat image;
  while (true) {
    // TODO(shoeffner): Stop if stream has no more frames, or return black images?
    *(this->video_capture) >> image;
    share_deque->push_back(image);
    std::this_thread::sleep_for(std::chrono::milliseconds(15));

    // TODO(shoeffner): Add consumer for image deque then remove the consumption here!
    if (share_deque->size() > 10) {
      share_deque->pop_front();
    }
  }
}

}  // namespace gaze
