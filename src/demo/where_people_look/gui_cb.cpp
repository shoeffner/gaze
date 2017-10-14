// Copyright 2017 Sebastian Höffner

#include "where_people_look/gui_cb.h"

#include <string>

#include "gtk/gtk.h"

#include "where_people_look/experiment.h"


namespace wpl {

namespace gui {

bool cb_finish_assistant(const GtkWidget*,
                         const GtkWidget* window) {
  gtk_window_present(GTK_WINDOW(window));
  gtk_window_fullscreen(GTK_WINDOW(window));
  return false;
}

bool cb_key_press(const GtkWidget*,
                  const GdkEventKey* event_key) {
  switch (event_key->keyval) {
    case GDK_KEY_F4:
      if (event_key->state & GDK_MOD1_MASK) {
        gtk_main_quit();
      }
      break;
    case GDK_KEY_q: case GDK_KEY_W:
      if (event_key->state & GDK_META_MASK) {
        gtk_main_quit();
      }
      break;
  }
  return false;
}

bool cb_update_config(GtkWidget* const widget,
                      Experiment* const experiment) {
  std::string widget_name = std::string(gtk_widget_get_name(widget));
  if (!widget_name.compare("subject_id")) {
    experiment->get_config()->set_subject_id(
      std::string(gtk_entry_get_text(GTK_ENTRY(widget))));
  } else if (!widget_name.compare("result_dir_path")) {
    experiment->get_config()->set_result_dir_path(
      std::string(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget))));
  } else if (!widget_name.compare("stimuli_dir_path")) {
    experiment->get_config()->set_stimuli_dir_path(
      std::string(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget))));
  }

  if (experiment->get_config()->assistant_data_complete()) {
    GtkWidget* grid = gtk_widget_get_parent(widget);
    GtkWidget* assistant = gtk_widget_get_ancestor(grid, GTK_TYPE_ASSISTANT);
    gtk_assistant_set_page_complete(GTK_ASSISTANT(assistant),
                                    grid, true);
  }
  return false;
}

void register_and_connect_callbacks(GtkBuilder* const builder,
                                    Experiment* const experiment) {
  gtk_builder_add_callback_symbols(builder,
      "cb_finish_assistant", G_CALLBACK(cb_finish_assistant),
      "cb_key_press", G_CALLBACK(cb_key_press),
      "cb_prepare_experiment", G_CALLBACK(Experiment::experiment_prepare),
      "cb_start_experiment", G_CALLBACK(Experiment::experiment_start),
      "cb_update_config", G_CALLBACK(cb_update_config),
      static_cast<void*>(0));

  gtk_builder_connect_signals(builder, experiment);
}

void set_css_style(GtkWidget* const window,
                   const char* const css_resource) {
  GtkStyleContext* style_context = gtk_widget_get_style_context(window);
  GtkCssProvider* style_provider = gtk_css_provider_new();
  gtk_css_provider_load_from_resource(style_provider, css_resource);
  gtk_style_context_add_provider(style_context,
      GTK_STYLE_PROVIDER(style_provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

}  // namespace gui

}  // namespace wpl
