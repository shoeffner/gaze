// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_

#include <queue>
#include <memory>
#include <mutex>  // NOLINT
#include <string>

#include "gaze/gaze.h"
#include "gtk/gtk.h"

#include "where_people_look/config.h"


namespace wpl {

/**
 * @class Experiment experiment.h "where_people_look/experiment.h"
 *
 * @brief Implements an experiment flow for Judd et al. (2009) \cite judd2009.
 *
 * The data acquisition experiment can be prepared, started, quitted, and
 * is divided into trials.
 *
 * @par Using this class
 *
 * The experiment should first be prepared (prepare()), then
 * started (start()). After that, it will go through each available
 * trial, recalibrate every 50 trials, and stop after all trials are done.
 *
 * Each trial lasts three seconds and there are pauses of one second between
 * the trials, during which a gray screen is shown.
 *
 * Before the first and after the last trial there is another two seconds
 * break.
 *
 * @note
 * Many static functions are just created to allow G_CALLBACK to call them
 * with appropriate Experiment instances. For example,
 * Experiment::experiment_trial calls trial()
 * on the provided experiment. However, some do extra checking:
 * Experiment::experiment_start will only start the experiment if the space key
 * was pressed and the experiment is not yet running.
 */
class Experiment {
  std::mutex mutex;

  Config* config;
  std::unique_ptr<gaze::GazeTracker> gaze_tracker;
  GtkImage* image;
  std::queue<std::string> trials;

  // State variables
  bool is_started = false;
  bool is_prepared = false;
  bool is_calibrating = false;

  // Configuration variables
  int calibration_after = 50;
  int calibration_countdown = this->calibration_after;
  int cooldown = 2000;
  int pause_duration = 1000;
  int trial_duration = 3000;
  int warmup = 2000;

  const void init_gaze_tracker();

  const void read_stimuli_list();

  static gboolean experiment_stop_trial(gpointer experiment);

  static gboolean experiment_start_trial(gpointer experiment);

 public:
    /**
     * Constructs an experiment.
     *
     * @param image The GtkImage* to be updated for each trial.
     * @param config The configuration. It is used to find the stimuli.
     */
    Experiment(GtkImage* const image, Config* const config);

    ~Experiment();

    /**
     * @returns a pointer to this experiment's config.
     */
    Config* const get_config() const;

    /** @name Experiment usage */
    //@{
    /**
     * Prepares the experiment.
     *
     * Preparations performed are
     *
     * - GazeTracker initialization
     * - Stimuli list generation
     */
    const void prepare();

    /**
     * Starts the experiment.
     * After a warmup time, the first trial starts.
     *
     * If this function is called on a started experiment, it simply returns.
     */
    const void start();
    //@}

    // TODO(shoeffner): Maybe this should become private?
    /** @name You should not call this manually. */
    //@{
    /**
     * Handles trials.
     *
     * This function should not be called manually.
     *
     * If a calibration is needed (because of the 50-trials-then-recalibrate
     * setting), this calls the calibration function and polls itself every
     * 500 millisecondss to try to continue the experiment with the next trial.
     *
     * If a calibration is already (or still) in progress, this simply returns
     * `true` to trigger another test.
     *
     * If there are no trials left, this issues a program quit after two seconds.
     *
     * Only if no calibration needs to be done or is in progress, and there are
     * still trials left, an actual trial is started:
     * A stimulus is presented for three seconds, followed by a one second pause.
     * This trial function issues a new trial to be performed afterwards and
     * returns `false` to not poll itself again.
     *
     * @returns true for retries during calibration, false otherwise.
     */
    const bool trial();
    //@}

    /**
     * Calibrates the GazeTracker.
     */
    static gboolean experiment_calibrate(gpointer experiment);

    /**
     * Calls trial() on the given experiment.
     *
     * @param experiment Must be a gpointer to an Experiment.
     * @returns false
     */
    static gboolean experiment_trial(gpointer experiment);

    /**
     * Gets the ancestor window of the experiment's image
     * (see Experiment(GtkImage*,Config*)) and issues a `destroy` signal.
     *
     * @param experiment Must be a gpointer to an Experiment.
     * @returns false
     */
    static gboolean experiment_quit_program(gpointer experiment);

    /**
     * Calls prepare() on the given experiment.
     *
     * @param assistant This widget is ignored but needed to bind the
     *                  G_CALLBACK properly.
     * @param experiment Must be a gpointer to an Experiment.
     * @returns false
     */
    static const bool experiment_prepare(const GtkWidget* const assistant,
                                         Experiment* const experiment);

    /**
     * Calls start() on the given experiment if the space key is event_key's
     * keyval and the experiment is not started.
     *
     * @param window This widget is ignored but needed to bind the
     *               G_CALLBACK properly.
     * @param event_key The event key to process.
     * @param experiment Must be a gpointer to an Experiment.
     * @returns false
     */
    static const bool experiment_start(const GtkWidget* const window,
                                       const GdkEventKey* const event_key,
                                       Experiment* const experiment);
};

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
