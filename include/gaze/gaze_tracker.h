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

/**
 * \class GazeTracker gaze_tracker.h "gaze/gaze_tracker.h"
 * \brief This class wraps the gaze tracking process.
 *
 * This is the GazeTracker, the heart of gaze. It allows
 * for easy initialization, tracking, and trial definitions.
 */
// TODO(shoeffner): Check for initialization in all members functions.
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
    /**
     * Constructs a GazeTracker instance. If you use this method, you must
     * call one of the initialization functions afterwards.
     */
    GazeTracker();

    /**
     * Constructs and initializes a GazeTracker instance for a video source.
     * See GazeTracker::init(std::string, std::string, std::string).
     *
     * @param source The video file path.
     * @param subject_id The subject identifier.
     * @param result_dir The directory to store the results.
     */
    GazeTracker(const int source,
                const std::string subject_id = "default_subject",
                const std::string result_dir = "./");

    /**
     * Constructs and initializes a GazeTracker instance for a video source.
     * See GazeTracker::init(std::string, std::string, std::string).
     *
     * @param source The video file path.
     * @param subject_id The subject identifier.
     * @param result_dir The directory to store the results.
     */
    GazeTracker(const std::string source,
                const std::string subject_id = "default_subject",
                const std::string result_dir = "./");

    ~GazeTracker();

    /**
     * Calibrates the GazeTracker.
     *
     * Note that currently there is no calibration taking place.
     */
    const void calibrate();

    /**
     * @returns the most recent frame from the capture source.
     */
    const cv::Mat get_current_frame() const;

    /** @name Trial handling */
    //@{
    /**
     * @returns the most recent calculated gaze point.
     */
    const std::pair<int, int> get_current_gaze_point() const;
    //@}

    /** @name Initialization
     *
     * These methods allow initialization of a GazeTracker instance.
     * If GazeTracker() was used to construct the instance, you must call
     * either of these functions before perfoming any actions.
     */
    //@{
    /**
     * Initializes the GazeTracker to use a webcam.
     *
     * It immediately starts tracking.
     *
     * @param source The webcam ID to be used.
     * @param subject_id The subject ID, used to store the data in the correct
     *                   file.
     * @param result_dir The directory where the results should be stored.
     */
    const void init(const int source,
                    const std::string subject_id,
                    const std::string result_dir = "./");

    /**
     * Initializes the GazeTracker to use a video.
     *
     * It immediately starts tracking.
     *
     * @param source The video file path to be used.
     * @param subject_id The subject ID, used to store the data in the correct
     *                   file.
     * @param result_dir The directory where the results should be stored.
     */
    const void init(const std::string source,
                    const std::string subject_id,
                    const std::string result_dir = "./");
    //@}

    /**
     * Prints information about the GazeTracker's capture source to std::cout.
     */
    const void print_capture_info() const;

    /**
     * Calls GazeTracker::print_capture_info() and prints information
     * about the subject and result directory.
     */
    const void print_info() const;

    /**
     * Opens a cv::namedWindow window named `GazeTracker Debug` which
     * shows the current captured frame. If no frame is available,
     * an empty (black) screen is shown.
     */
    const void show_debug_screen() const;

    /** @name Trial handling */
    //@{
    /**
     * Makes the GazeTracker associate its output data with the `identifier`
     * until either GazeTracker::stop_trial() is called or a new trial is
     * started by calling this function with a different identifier.
     *
     * The identifiers are not checked for uniqueness! It is very well
     * possible to use the same identifier multiple tames, analyzing the
     * data correctly is then up to you.
     *
     * @param identifier The trial identifier.
     */
    const void start_trial(const std::string identifier);

    /**
     * Stops associating data with any specific trial.
     */
    const void stop_trial();
    //@}

    /** @name Trial handling
     *
     * These functions are used to partition an experiment into trials.
     * When you start a trial, call the GazeTracker::start_trial(const std::string)
     * method. After the trial you should issue GazeTracker::stop_trial().
     *
     * Note that the GazeTracker keeps tracking until you destroy the instance,
     * the only difference in calling the start and stop trial functions is
     * that the identifier will be available next to the data for the specific trial.
     *
     * GazeTracker::get_current_gaze_point() can be used for feedback loops.
     */
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_GAZE_TRACKER_H_
