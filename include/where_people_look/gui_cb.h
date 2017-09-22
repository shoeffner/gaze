// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_

#include "gtk/gtk.h"

#include "where_people_look/experiment.h"


namespace wpl {

namespace gui {

const bool cb_finish_assistant(const GtkWidget* assistant,
                               const GtkWidget* window,
                               Experiment* experiment);

const bool cb_key_press(const GtkWidget* widget,
                        const GdkEventKey* event_key);

const bool cb_update_config(GtkWidget* widget, Experiment* experiment);

const void register_and_connect_callbacks(GtkBuilder* builder,
                                          Experiment* experiment);

const void set_css_style(GtkWidget* window, const char* css_resource);

}  // namespace gui

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_
