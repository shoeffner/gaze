// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/source_capture.h"

#include <chrono>  // NOLINT
#include <memory>
#include <string>
#include <thread>  // NOLINT

#include "dlib/opencv.h"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

void SourceCapture::init() {
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

int SourceCapture::get_frames_per_second() const {
  return this->fps;
}

int SourceCapture::get_height() const {
  return this->height;
}

int SourceCapture::get_width() const {
  return this->width;
}

void SourceCapture::process(util::Data* data) {
  bool end = false;
  while (!end) {
    end = this->video_capture->read(data->source_image);
    dlib::assign_image(
        data->image,
        dlib::cv_image<dlib::bgr_pixel>(data->source_image));
    if (!end) {
      std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
  }
}

}  // namespace pipeline

}  // namespace gaze
