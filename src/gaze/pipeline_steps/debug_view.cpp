// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/debug_view.h"

#include <string>

#include "opencv2/highgui.hpp"

#include "gaze/util/data.h"

#include <iostream>
namespace gaze {

namespace pipeline {

int DebugView::counter = 0;

DebugView::DebugView(const std::function<cv::Mat(util::Data*)> accessor) {
  this->accessor = accessor;
  this->window_name = "Debug View " + std::to_string(++DebugView::counter);
  cv::namedWindow(this->window_name);
}

void DebugView::process(util::Data* data) {
  cv::imshow(this->window_name, this->accessor(data));
}

DebugView::~DebugView() {
  cv::destroyWindow(this->window_name);
}

}  // namespace pipeline

}  // namespace gaze
