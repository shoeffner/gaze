// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_GUI_DEBUG_WINDOW_H_
#define INCLUDE_GAZE_GUI_DEBUG_WINDOW_H_

#include <memory>
#include <vector>

#include "dlib/gui_core.h"
#include "dlib/gui_widgets.h"

#include "gaze/pipeline.h"
#include "gaze/util/data.h"


namespace gaze {

namespace gui {

/**
 * @class DebugWindow debug_window.h "gaze/gui/debug_window.h"
 * @brief A window to inspect computation results.
 */
class DebugWindow final : public dlib::drawable_window {
  int current_tab = 0;
  util::Data data;
  dlib::button pause_button;
  bool paused;
  Pipeline* pipeline;
  std::vector<PipelineStep*> pipeline_steps;
  dlib::tabbed_display pipeline_tabs;
  std::vector<std::shared_ptr<dlib::widget_group>> widgets;

  const int w_height = 720;
  const int w_margin = 5;
  const int w_width = 1280;

 public:
  /**
   * Creates a DebugWindow.
   *
   * @param pipeline The pipeline to inspect.
   */
  //@{
  explicit DebugWindow(Pipeline* pipeline);
  ~DebugWindow();
  //@}

  /**
   * Processes the data to be visualized.
   *
   * Calls the visualize function of the PipelineStep of the active tab.
   *
   * @param idx (Optional): If this is a value other than -1,
   *            the tab with this index is updated instead of the current tab.
   */
  void process_data(int idx = -1);

  /**
   * Checks the event type (@see Events) and handles events.
   * @param event_data data passed along with the event.
   * @param event_type the event type (@see Events).
   */
  //@{
  void on_user_event(void *event_data, int event_type) override;
  //@}
};

/**
 * Creates and opens a DebugWindow.
 *
 * @param pipeline The pipeline to inspect.
 */
void open_debug_window(Pipeline* pipeline);

}  // namespace gui

}  // namespace gaze

#endif  // INCLUDE_GAZE_GUI_DEBUG_WINDOW_H_
