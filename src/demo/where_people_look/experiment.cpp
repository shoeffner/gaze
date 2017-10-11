// Copyright 2017 Sebastian Höffner

#include "where_people_look/experiment.h"

#include <memory>
#include <mutex>  // NOLINT
#include <string>

#include "boost/filesystem.hpp"
#include "gaze/gaze.h"
#include "gtk/gtk.h"

#include "where_people_look/config.h"


namespace wpl {

const void Experiment::init_gaze_tracker() {
  this->gaze_tracker->init(0,
      this->config->get_subject_id(),
      this->config->get_result_dir_path().string());
  // this->gaze_tracker->print_info();
}

const void Experiment::read_stimuli_list() {
  boost::filesystem::directory_iterator
    iter(this->config->get_stimuli_dir_path());
  for (boost::filesystem::directory_entry entry : iter) {
    if (boost::filesystem::is_regular_file(entry)
        && !boost::filesystem::extension(entry).compare(".jpeg")) {
      this->trials.push(entry.path().string());
    }
  }
}

gboolean Experiment::experiment_stop_trial(gpointer experiment) {
  Experiment* exp = static_cast<Experiment*>(experiment);
  exp->gaze_tracker->stop_trial();
  gtk_widget_hide(GTK_WIDGET(exp->image));
  return false;
}

gboolean Experiment::experiment_start_trial(gpointer experiment) {
  Experiment* exp = static_cast<Experiment*>(experiment);
  std::string image_file = exp->trials.front();
  exp->trials.pop();
  gtk_image_set_from_file(exp->image, image_file.c_str());
  gtk_widget_show(GTK_WIDGET(exp->image));
  exp->gaze_tracker->start_trial(image_file);
  return false;
}

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
  std::lock_guard<std::mutex> lock(this->mutex);
  if (this->is_prepared) {
    return;
  }
  this->init_gaze_tracker();
  this->read_stimuli_list();
  this->is_prepared = true;
}

const void Experiment::start() {
  std::lock_guard<std::mutex> lock(this->mutex);
  if (this->is_started) {
    return;
  }
  g_timeout_add(this->warmup, Experiment::experiment_trial, this);
  this->is_started = true;
}

const bool Experiment::trial() {
  std::lock_guard<std::mutex> lock(this->mutex);
  if (this->is_calibrating) {
    return true;
  }
  if (this->calibration_countdown-- == 0) {
    this->is_calibrating = true;
    this->calibration_countdown = this->calibration_after;
    g_timeout_add(0, Experiment::experiment_calibrate, this);
    g_timeout_add(500, Experiment::experiment_trial, this);
  } else {
    if (!this->trials.empty()) {
      g_timeout_add(0, Experiment::experiment_start_trial, this);
      g_timeout_add(this->trial_duration, Experiment::experiment_stop_trial,
          this);
      g_timeout_add(this->trial_duration + this->pause_duration,
          Experiment::experiment_trial, this);
    } else {
      g_timeout_add(this->cooldown, Experiment::experiment_quit_program, this);
    }
  }
  return false;
}

gboolean Experiment::experiment_calibrate(gpointer experiment) {
  Experiment* exp = static_cast<Experiment*>(experiment);
  exp->is_calibrating = false;
  exp->gaze_tracker->calibrate();
  return false;
}

gboolean Experiment::experiment_quit_program(gpointer experiment) {
  Experiment* exp = static_cast<Experiment*>(experiment);
  GtkWidget* window = gtk_widget_get_ancestor(GTK_WIDGET(exp->image),
      GTK_TYPE_WINDOW);
  g_signal_emit_by_name(window, "destroy");
  return false;
}

gboolean Experiment::experiment_trial(gpointer experiment) {
  return static_cast<Experiment*>(experiment)->trial();
}

const bool Experiment::experiment_prepare(const GtkWidget* const assistant,
                                          Experiment* const experiment) {
  experiment->prepare();
  return false;
}

const bool Experiment::experiment_start(const GtkWidget* const window,
                                        const GdkEventKey* const event_key,
                                        Experiment* const experiment) {
  if (event_key->keyval == GDK_KEY_space) {
    if (!experiment->is_started) {
      experiment->start();
    }
  }
  return false;
}

}  // namespace wpl
