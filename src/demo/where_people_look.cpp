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
#include <map>

#include "gaze/gaze.h"
#include "gtk/gtk.h"


namespace {

namespace gui {

  static void cb_key_press(GtkWidget* widget, GdkEventKey* event_key) {
    switch (event_key->keyval) {
      case GDK_KEY_F4:
        if (event_key->state & GDK_MOD1_MASK) {
          std::exit(0);
        }
        break;
      case GDK_KEY_q: case GDK_KEY_W:
        if (event_key->state & GDK_META_MASK) {
          std::exit(0);
        }
        break;
    }
  }

  static void cb_activate(GtkApplication* app, gpointer data_ignored) {
    GtkWidget* window = gtk_application_window_new(app);

    // gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    g_signal_connect(window, "key-press-event", G_CALLBACK(cb_key_press), NULL);

    gtk_window_fullscreen(GTK_WINDOW(window));
    // TODO(shoeffner): Window needs focus

    gtk_widget_show_all(window);
  }

}  // namespace gui

}  // namespace

int main(int argc, char** argv) {
  GtkApplication* app = gtk_application_new("de.uos.gaze.wpl",
      G_APPLICATION_FLAGS_NONE);

  g_signal_connect(app, "activate", G_CALLBACK(::gui::cb_activate), NULL);


  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
