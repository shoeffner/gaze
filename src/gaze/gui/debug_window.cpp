// Copyright 2017 Sebastian Höffner

#include "gaze/gui/debug_window.h"

#include <memory>
#include <thread>  // NOLINT

#include "dlib/geometry.h"
#include "dlib/gui_core.h"
#include "dlib/gui_widgets.h"

#include "gaze/gui/event_manager.h"
#include "gaze/pipeline.h"


namespace gaze {

namespace gui {

DebugWindow::DebugWindow(Pipeline* pipeline)
    : dlib::drawable_window(false, false),
      pipeline(pipeline) {
  this->set_size(640, 360);
  this->set_title("GazeTracker DebugWindow");
  this->show();
  EventManager::instance().subscribe(this);
}

DebugWindow::~DebugWindow() {
  EventManager::instance().unsubscribe(this);
  this->close_window();
}

void DebugWindow::process_data() {
  for (PipelineStep* step : this->pipeline->get_steps()) {
    step->visualize(this->data);
  }
  dlib::assign_image(
      this->data.image,
      dlib::cv_image<dlib::bgr_pixel>(this->data.source_image));
}

void DebugWindow::on_user_event(void*, int event_type) {
  switch (event_type) {
    case Events::PIPELINE_DATA_UPDATED:
      this->data = this->pipeline->get_data();
      this->process_data();
      this->invalidate_rectangle(
          dlib::rectangle(0, 0,
                          this->data.image.nc(),
                          this->data.image.nr()));
      break;
  }
}

void DebugWindow::paint(const dlib::canvas& canvas) {
  dlib::draw_image(canvas, canvas, this->data.image);
}

namespace {

/**
 * Actual debug window routine to be called by open_debug_window().
 * It creates an instance of a DebugWindow and starts its main loop.
 */
void _open_debug_window(Pipeline* pipeline) {
  DebugWindow(pipeline).wait_until_closed();
}

}  // namespace

void open_debug_window(Pipeline* pipeline) {
  std::thread(&_open_debug_window, pipeline).detach();
}

}  // namespace gui

}  // namespace gaze
