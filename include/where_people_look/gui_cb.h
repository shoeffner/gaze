// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_

#include "gtk/gtk.h"

#include "where_people_look/experiment.h"


namespace wpl {

namespace gui {

bool cb_finish_assistant(const GtkWidget* assistant,
                         const GtkWidget* window);

bool cb_key_press(const GtkWidget* widget,
                  const GdkEventKey* event_key);

bool cb_update_config(GtkWidget* const widget,
                      Experiment* const experiment);

void register_and_connect_callbacks(GtkBuilder* const builder,
                                    Experiment* const experiment);

void set_css_style(GtkWidget* const window,
                   const char* const css_resource);

}  // namespace gui

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_
