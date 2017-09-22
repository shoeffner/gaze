// Copyright 2017 Sebastian Höffner

#include "where_people_look/experiment.h"

#include <iostream>

#include "gtk/gtk.h"

#include "where_people_look/config.h"


namespace wpl {

Experiment::Experiment(GtkImage* image, Config* config) {
  this->image = image;
  this->config = config;
}

Config* Experiment::get_config() const {
  return config;
}

const void Experiment::prepare() {
}

const void Experiment::start() {
  running = true;
}

bool Experiment::start_experiment(GtkWidget* widget,
                                  GdkEventKey* event_key,
                                  Experiment* experiment) {
  if (!experiment->running && event_key->keyval == GDK_KEY_space) {
    experiment->start();
  }
  return false;
}

}  // namespace wpl
