#ifndef INCLUDE_GAZE_GAZE_TRACKER_H_
#define INCLUDE_GAZE_GAZE_TRACKER_H_

#include <string>
#include <utility>
#include <vector>

#include "opencv2/core.hpp"

#include "gaze/pipeline.h"
#include "gaze/pipeline_step.h"
#include "gaze/pipeline_steps/source_capture.h"


namespace gaze {

/**
 * @class GazeTracker gaze_tracker.h "gaze/gaze_tracker.h"
 * @brief This class wraps the gaze tracking process.
 *
 * This is the GazeTracker, the heart of gaze. It allows
 * for easy initialization, tracking, and trial definitions.
 */
class GazeTracker {
  std::string current_trial_id;
  bool debug;
  bool initialized;
  Pipeline* pipeline;
  std::vector<PipelineStep*> pipeline_steps;
  std::string subject_id;
  std::string video_source;

  /**
   * Initializes the thread for the processing pipeline.
   *
   * @param subject_id The subject identifier.
   */
  void init_pipeline(const std::string subject_id);

 public:
    /**
     * Constructs and initializes a GazeTracker instance for a video source.
     * See GazeTracker::init(std::string, std::string, std::string).
     *
     * @param subject_id The subject identifier.
     * @param debug Starts the gaze_tracker in debug mode, bringing up
     *              additional debuggin screens.
     */
    GazeTracker(const std::string subject_id = "default_subject",
                const bool debug = false);

    ~GazeTracker();

    /**
     * Calibrates the GazeTracker.
     *
     * @note Currently there is no calibration phase.
     */
    void calibrate();

    /** @name Data access */
    //@{
    /**
     * @returns the most recent calculated gaze point. If not initialized,
     *          returns @f$(-1, -1)@f$.
     */
    const std::pair<int, int> get_current_gaze_point() const;
    //@}

    /** @name Initialization
     *
     * This method allows initialization of a GazeTracker instance.
     * If GazeTracker() was used to construct the instance, you must call
     * either of these functions before perfoming any actions.
     */
    //@{
    /**
     * Initializes the GazeTracker.
     *
     * It immediately starts tracking.
     *
     * @param subject_id The subject ID, used to store the data in the correct
     *                   file.
     * @param debug Starts the gaze_tracker in debug mode, bringing up
     *              additional debuggin screens.
     */
    void init(const std::string subject_id,
              const bool debug = false);
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
     */
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
    void start_trial(const std::string identifier);

    /**
     * Stops associating data with any specific trial.
     */
    void stop_trial();
    //@}
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_GAZE_TRACKER_H_
