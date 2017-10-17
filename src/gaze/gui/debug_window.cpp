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
      pause_button(*this),
      pipeline(pipeline),
      pipeline_tabs(*this) {
  this->set_size(this->w_width, this->w_height);
  this->set_title("GazeTracker DebugWindow");


  this->pause_button.set_pos(this->w_margin, this->w_margin);
  this->pause_button.set_name("Pause");


  this->pipeline_tabs.set_pos(this->pause_button.left(),
                              this->pause_button.bottom() + this->w_margin);
  this->pipeline_tabs.set_size(this->w_width - 2 * this->w_margin,
                               this->w_height - 3 * this->w_margin
                                   - this->pause_button.height());
  this->pipeline_tabs.set_number_of_tabs(pipeline->get_steps().size());

  for (unsigned long i = 0; i < pipeline->get_steps().size(); ++i) {
    // Create widget and assign it to the pipeline step
    std::shared_ptr<PipelineStepWidget> widget(new PipelineStepWidget(*this));
    // Roughly fit the borders and button sizes, it's not perfect, but well
    widget->set_size(this->pipeline_tabs.width() - 3 * this->w_margin,
                     this->pipeline_tabs.height() - 3 * this->w_margin
                       - this->pause_button.height());
    pipeline->get_steps()[i]->set_widget(widget);

    // Create widget group, add widget to it
    std::shared_ptr<dlib::widget_group> widget_group_ptr(
        new dlib::widget_group(*this));
    this->widgets.push_back(widget_group_ptr);
    widget_group_ptr->add(*widget.get(), this->w_margin, this->w_margin);

    // Assign name and add widget group to tab
    this->pipeline_tabs.set_tab_name(i, pipeline->get_steps()[i]->name());
    this->pipeline_tabs.set_tab_group(i, *widget_group_ptr.get());
  }

  this->show();
  EventManager::instance().subscribe(this);
}

DebugWindow::~DebugWindow() {
  EventManager::instance().unsubscribe(this);
  for (PipelineStep* step : this->pipeline->get_steps()) {
    step->set_widget(nullptr);
  }
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
      // TODO(shoeffner): implement pause button behaviour
      this->data = this->pipeline->get_data();
      // TODO(shoeffner): Only process visible tab?
      this->process_data();
      this->invalidate_rectangle(this->pipeline_tabs.get_rect());
      break;
  }
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
