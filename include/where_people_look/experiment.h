// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_

#include "where_people_look/config.h"

#include "gtk/gtk.h"

namespace wpl {

class Experiment {
  GtkImage* image;
  Config* config;

  bool running = false;

 public:
    explicit Experiment(GtkImage* image, Config* config);

    Config* get_config() const;

    const void prepare();

    const void start();

    static bool start_experiment(GtkWidget* widget,
                                 GdkEventKey* event_key,
                                 Experiment* experiment);
};

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
