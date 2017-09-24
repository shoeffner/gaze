// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_GAZE_H_
#define INCLUDE_GAZE_GAZE_H_

#include <memory>
#include <string>

#include "opencv2/videoio.hpp"


namespace gaze {

class GazeTracker {
  enum SourceType { WEBCAM, VIDEO };

  std::string current_trial_id;
  bool initialized;
  std::string result_dir;
  SourceType source_type;
  std::string subject_id;
  std::unique_ptr<cv::VideoCapture> video_capture;
  std::string video_source;

 public:
    GazeTracker();

    explicit GazeTracker(const int source);

    explicit GazeTracker(const std::string source);

    ~GazeTracker();

    const void calibrate();

    const void init(const int source,
                    const std::string subject_id,
                    const std::string result_dir = "./");

    const void init(const std::string source,
                    const std::string subject_id,
                    const std::string result_dir = "./");

    const void print_capture_info() const;

    const void print_info() const;

    const void show_debug_screen() const;

    const void start_trial(const std::string identifier);

    const void stop_trial();
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_GAZE_H_
