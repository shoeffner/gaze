// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_GUI_DEBUG_WINDOW_H_
#define INCLUDE_GAZE_GUI_DEBUG_WINDOW_H_

#include <memory>

#include "dlib/gui_core.h"
#include "dlib/gui_widgets.h"

#include "gaze/pipeline.h"
#include "gaze/util/data.h"


namespace gaze {

namespace gui {

class DebugWindow final : public dlib::drawable_window {
  util::Data data;
  Pipeline* pipeline;

 public:
  explicit DebugWindow(Pipeline* pipeline);
  ~DebugWindow();

  void on_user_event(void *event_data, int event_type) override;

  void paint(const dlib::canvas& canvas) override;
};

void open_debug_window(Pipeline* pipeline);

}  // namespace gui

}  // namespace gaze

#endif  // INCLUDE_GAZE_GUI_DEBUG_WINDOW_H_
