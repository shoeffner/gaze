// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_GAZE_TRACKER_H_
#define INCLUDE_GAZE_GAZE_TRACKER_H_

#include <memory>
#include <string>
#include <thread>  // NOLINT
#include <utility>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include "gaze/source_capture.h"
#include "gaze/util/spmc_deque.h"


namespace gaze {

class GazeTracker {
  enum SourceType { WEBCAM, VIDEO };

  std::string current_trial_id;
  bool initialized;
  std::string result_dir;
  util::SPMCDeque<cv::Mat>* source_image_queue;
  SourceCapture* source_capture;
  std::thread* source_capture_thread;
  SourceType source_type;
  std::string subject_id;
  std::string video_source;

 public:
    GazeTracker();

    explicit GazeTracker(const int source,
                         const std::string subject_id = "default_subject",
                         const std::string result_dir = "./");

    explicit GazeTracker(const std::string source,
                         const std::string subject_id = "default_subject",
                         const std::string result_dir = "./");

    ~GazeTracker();

    const void calibrate();

    const cv::Mat get_current_frame() const;

    const std::pair<int, int> get_current_gaze_point() const;

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

#endif  // INCLUDE_GAZE_GAZE_TRACKER_H_
