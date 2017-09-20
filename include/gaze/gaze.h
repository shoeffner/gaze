// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_GAZE_H_
#define INCLUDE_GAZE_GAZE_H_

#include <memory>
#include <string>

#include "opencv2/videoio.hpp"


namespace gaze {

class GazeTracker {
  enum SourceType { WEBCAM, VIDEO };

  SourceType source_type;
  std::unique_ptr<cv::VideoCapture> video_capture;
  std::string video_source;


 public:
    explicit GazeTracker(std::string);

    ~GazeTracker();

    void print_capture_info();

    void show_debug_screen();
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_GAZE_H_
