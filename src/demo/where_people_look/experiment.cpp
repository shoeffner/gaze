// Copyright 2017 Sebastian Höffner

#include "where_people_look/experiment.h"

#include <memory>
#include <string>

#include "boost/filesystem.hpp"
#include "gaze/gaze.h"
#include "gtk/gtk.h"

#include "where_people_look/config.h"


namespace wpl {

Experiment::Experiment(GtkImage* const image, Config* const config) {
  this->image = image;
  this->config = config;
  this->gaze_tracker = std::unique_ptr<gaze::GazeTracker>(
      new gaze::GazeTracker());
}

Experiment::~Experiment() {
}

Config* const Experiment::get_config() const {
  return this->config;
}

const void Experiment::prepare() {
  this->mutex.lock();
  this->gaze_tracker->init(0);
  this->gaze_tracker->print_capture_info();
  this->mutex.unlock();
}

const void Experiment::start() {
  this->mutex.lock();
  this->mutex.unlock();
}

const bool Experiment::prepare_experiment(const GtkWidget* const assistant,
                                          Experiment* const experiment) {
  experiment->prepare();
  return false;
}

const bool Experiment::start_experiment(const GtkWidget* const window,
                                        const GdkEventKey* const event_key,
                                        Experiment* const experiment) {
  experiment->start();
  return false;
}

}  // namespace wpl
