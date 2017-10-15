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

/**
 * @class DebugWindow debug_window.h "gaze/gui/debug_window.h"
 * @brief A window to inspect computation results.
 */
class DebugWindow final : public dlib::drawable_window {
  util::Data data;
  Pipeline* pipeline;

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
   * @deprecated Should be replaced "soon" by a renderer not modifying
   *             OpenCV matrices.
   *
   * Calls the visualize functions of all pipeline steps, and copies
   * the modified cv::Mat source_image to the dlib::array2d image.
   */
  void process_data();

  /**
   * Checks the event type (@see Events) and handles events.
   * @param event_data data passed along with the event.
   * @param event_type the event type (@see Events).
   */
  //@{
  void on_user_event(void *event_data, int event_type) override;

  /**
   * Repaints the window.
   *
   * @param canvas the canvas.
   */
  void paint(const dlib::canvas& canvas) override;
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
