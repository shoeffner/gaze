// Copyright 2017 Sebastian Höffner

#include "gaze/gui/debug_window.h"

#include <memory>
#include <thread>  // NOLINT

#include "dlib/gui_core.h"
#include "dlib/gui_widgets.h"

#include "gaze/pipeline.h"


namespace gaze {

namespace gui {

DebugWindow::DebugWindow(Pipeline* pipeline)
    : pipeline(pipeline) {
  this->set_size(640, 400);
  this->set_title("GazeTracker DebugWindow");
  this->show();
  this->pipeline->set_debug_window(this);
}

DebugWindow::~DebugWindow() {
  this->close_window();
  // TODO(shoeffner): Proper way to notify pipeline of destruction?
  std::this_thread::sleep_for(std::chrono::seconds(3));
}

void DebugWindow::on_user_event(void*, int event_type) {
  switch (event_type) {
    case 0:  // Pipeline has new results
      this->data = this->pipeline->get_data();
      this->invalidate_rectangle(dlib::rectangle(0, 0, 640, 480));
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
