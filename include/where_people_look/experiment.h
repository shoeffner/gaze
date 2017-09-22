// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_

#include <memory>
#include <mutex>  // NOLINT

#include "gaze/gaze.h"
#include "gtk/gtk.h"

#include "where_people_look/config.h"


namespace wpl {

class Experiment {
  std::mutex mutex;

  GtkImage* image;
  Config* config;
  std::unique_ptr<gaze::GazeTracker> gaze_tracker;

 public:
    explicit Experiment(GtkImage* const image, Config* const config);

    ~Experiment();

    Config* const get_config() const;

    const void prepare();

    const void start();

    static const bool prepare_experiment(const GtkWidget* const assistant,
                                         Experiment* const experiment);

    static const bool start_experiment(const GtkWidget* const window,
                                       const GdkEventKey* const event_key,
                                       Experiment* const experiment);
};

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
