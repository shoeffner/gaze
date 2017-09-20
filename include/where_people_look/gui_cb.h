// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_

#include "gtk/gtk.h"

namespace wpl {

namespace gui {

const void cb_key_press(const GtkWidget* widget,
                               const GdkEventKey* event_key);

const void cb_finish_assistant(const GtkWidget* assistant,
                                      const GtkWidget* window);

const void set_css_style(GtkWidget* window, const char* css_resource);

const void register_and_connect_callbacks(GtkBuilder* builder);

}  // namespace gui

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_GUI_CB_H_
