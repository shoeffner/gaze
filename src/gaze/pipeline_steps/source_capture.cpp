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

SourceCapture::SourceCapture(int source) : video_capture(source) {
  this->name = "Source Capture";
  // Set goal FPS to 60, still often webcams cap much lower.
  // (Not that the tracker would achieve this speed...)
  this->video_capture.set(cv::CAP_PROP_FPS, 60.0);
  this->video_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640.0);
  this->video_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 360.0);
}

SourceCapture::SourceCapture(std::string source) : video_capture(source) {
}

SourceCapture::~SourceCapture() {
  this->video_capture.release();
}

void SourceCapture::process(util::Data& data) {
  bool end = false;
  while (!end) {
    end = this->video_capture.read(data.source_image);
    dlib::assign_image(
        data.image,
        dlib::cv_image<dlib::bgr_pixel>(data.source_image));
    if (!end) {
      std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
  }
}

void SourceCapture::visualize(util::Data& data) {
  this->widget->set_image(data.image);
}

}  // namespace pipeline

}  // namespace gaze
