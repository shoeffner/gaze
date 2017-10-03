// Copyright 2017 Sebastian Höffner
/**
 * @namespace wpl
 * @brief Data acquisition after Judd et al. (2009) \cite judd2009
 *
 * This is a reimplementation of the data acquisition experiment described in
 * section 2.1 of Judd et al.: Learning to Predict Where Humans Look (2009)
 * \cite judd2009.
 *
 * Check out their fantastic website, showing off their results:
 * <a href="http://people.csail.mit.edu/tjudd/WherePeopleLook/interactiveWebsite/seeFixations.html">Interactive fixations</a>
 */

#include "gaze/gaze.h"
#include "gtk/gtk.h"

#include "where_people_look/config.h"
#include "where_people_look/experiment.h"
#include "where_people_look/gui_cb.h"


int main(int argc, char** argv) {
  // Initialize program
  gtk_init(&argc, &argv);

  GtkBuilder* builder = gtk_builder_new_from_resource("/wpl/where_people_look.ui");  // NOLINT
  GtkWidget* assistant = GTK_WIDGET(gtk_builder_get_object(builder, "SetupWindow"));  // NOLINT
  GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
  GtkWidget* image = GTK_WIDGET(gtk_builder_get_object(builder, "Stimulus"));

  wpl::Config* config = new wpl::Config();
  wpl::Experiment* experiment = new wpl::Experiment(GTK_IMAGE(image), config);

  wpl::gui::set_css_style(window, "/wpl/where_people_look.css");
  wpl::gui::register_and_connect_callbacks(builder, experiment);

  gtk_main();

  delete config;
  delete experiment;

  return 0;
}
