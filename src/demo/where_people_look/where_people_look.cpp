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

static const void cb_key_press(const GtkWidget* widget,
                               const GdkEventKey* event_key) {
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

const void register_and_connect_callbacks(GtkBuilder* builder) {
  gtk_builder_add_callback_symbol(builder,
      "cb_key_press", G_CALLBACK(cb_key_press));

  gtk_builder_connect_signals(builder, NULL);
}

}  // namespace gui

}  // namespace


int main(int argc, char** argv) {
  // Initialize program
  gtk_init(&argc, &argv);

  GtkBuilder* builder = gtk_builder_new_from_resource("/wpl/where_people_look.ui");  // NOLINT
  ::gui::register_and_connect_callbacks(builder);

  GObject* window = gtk_builder_get_object(builder, "MainWindow");
  gtk_window_present(GTK_WINDOW(window));

  g_object_unref(builder);

  // Start GUI loop
  gtk_main();

  return 0;
}
