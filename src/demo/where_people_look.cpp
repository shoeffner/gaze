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

}  // namespace gui

}  // namespace


int main(int argc, char** argv) {
  gtk_init(&argc, &argv);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  g_signal_connect(window, "key-press-event", G_CALLBACK(::gui::cb_key_press),
      NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_fullscreen(GTK_WINDOW(window));
  gtk_window_present(GTK_WINDOW(window));

  gtk_main();

  g_object_unref(window);

  return 0;
}
