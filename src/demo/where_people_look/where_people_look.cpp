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

static bool cb_swap_image(GtkWidget* image) {
  gtk_image_set_from_file(GTK_IMAGE(image),
      "./wpl_stimuli/syntheticData1.jpeg");
  return true;
}

// TODO(shoeffner): Remove this when using CSS.
static GdkRGBA get_color(double r, gdouble g, double b, double a) {
  GdkRGBA color;
  color.red = r;
  color.green = g;
  color.blue = b;
  color.alpha = a;
  return color;
}

}  // namespace gui

}  // namespace


int main(int argc, char** argv) {
  // Initialize program
  gtk_init(&argc, &argv);

  // Prepare window properties
  // TODO(shoeffner): Use proper ui builder
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget* image = gtk_image_new_from_file("./wpl_stimuli/i1843924285.jpeg");

  gtk_container_add(GTK_CONTAINER(window), image);
  gtk_window_fullscreen(GTK_WINDOW(window));
  gtk_widget_show_all(window);
  gtk_window_set_keep_above(GTK_WINDOW(window), true);

  // TODO(shoeffner): Use CSS instead of deprecated gtk_widget_override...
  GdkRGBA gray = ::gui::get_color(.3, .3, .3, 1);
  gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &gray);

  // Set callbacks
  g_signal_connect(window, "key-press-event", G_CALLBACK(::gui::cb_key_press),
      NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  // TODO(shoeffner): Figure out a proper way to swap images
  // g_timeout_add_seconds(3, ::gui::cb_swap_image, &image);

  // Start GUI loop
  gtk_main();

  // Clean up
  // g_object_unref(image);
  // g_object_unref(window);

  return 0;
}
