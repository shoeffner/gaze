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

  bool initialized;


 public:
    GazeTracker();

    explicit GazeTracker(const int source);

    explicit GazeTracker(const std::string source);

    ~GazeTracker();

    const void init(const int source);

    const void init(const std::string source);

    const void print_capture_info() const;

    const void show_debug_screen() const;
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_GAZE_H_
