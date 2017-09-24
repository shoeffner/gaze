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
    explicit Experiment(GtkImage* const image, Config* const config);

    ~Experiment();

    Config* const get_config() const;

    const void prepare();

    const void start();

    const bool trial();

    static gboolean experiment_calibrate(gpointer experiment);

    static gboolean experiment_trial(gpointer experiment);

    static gboolean experiment_quit_program(gpointer experiment);

    static const bool experiment_prepare(const GtkWidget* const assistant,
                                         Experiment* const experiment);

    static const bool experiment_start(const GtkWidget* const window,
                                       const GdkEventKey* const event_key,
                                       Experiment* const experiment);
};

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
