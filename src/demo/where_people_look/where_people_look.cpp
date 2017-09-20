// Copyright 2017 Sebastian Höffner
// This is a crude reimplementation of the data gathering experiment described
// in section 2.1 of Judd et al.: Learning to Predict Where Humans Look (2009)
//
// @InProceedings{Judd_2009,
//   author    = {Tilke Judd and Krista Ehinger and Fr{\'e}do Durand and
//                Antonio Torralba},
//   title     = {Learning to Predict Where Humans Look},
//   booktitle = {IEEE International Conference on Computer Vision (ICCV)},
//   year      = {2009}
// }
//
// Check out their fantastic website, showing off their results:
// http://people.csail.mit.edu/tjudd/WherePeopleLook/interactiveWebsite/seeFixations.html

#include <iostream>

#include "gaze/gaze.h"
#include "gtk/gtk.h"

#include "where_people_look/config.h"
#include "where_people_look/gui_cb.h"

int main(int argc, char** argv) {
  // Initialize program
  gtk_init(&argc, &argv);

  GtkBuilder* builder = gtk_builder_new_from_resource("/wpl/where_people_look.ui");  // NOLINT
  GtkWidget* assistant = GTK_WIDGET(gtk_builder_get_object(builder, "SetupWindow"));  // NOLINT
  GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
  GtkWidget* image = GTK_WIDGET(gtk_builder_get_object(builder, "Image"));

  wpl::WPLConfig* config = new wpl::WPLConfig();

  wpl::gui::set_css_style(window, "/wpl/where_people_look.css");
  wpl::gui::register_and_connect_callbacks(builder, config);

  g_object_unref(builder);

  gtk_main();

  return 0;
}
