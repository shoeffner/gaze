// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_SOURCE_CAPTURE_H_
#define INCLUDE_GAZE_SOURCE_CAPTURE_H_

#include <memory>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include "gaze/util/spmc_deque.h"


namespace gaze {

class SourceCapture {
  cv::Mat empty_frame;
  int fps;
  int height;
  std::unique_ptr<cv::VideoCapture> video_capture;
  int width;

 public:
    explicit SourceCapture(const int source);

    explicit SourceCapture(const std::string source);

    ~SourceCapture();

    const cv::Mat get_empty_frame() const;

    const int get_frames_per_second() const;

    const int get_height() const;

    const int get_width() const;

    const void init();

    const void operator()(util::SPMCDeque<cv::Mat>* const share_deque) const;
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_SOURCE_CAPTURE_H_
