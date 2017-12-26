#include "gaze/gui/debug_window.h"

#include <memory>
#include <sstream>
#include <thread>  // NOLINT
#include <utility>

#include "dlib/geometry.h"
#include "dlib/gui_core.h"
#include "dlib/gui_widgets.h"
#include "dlib/unicode.h"

#include "gaze/gui/event_manager.h"
#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline.h"


namespace gaze {

namespace gui {

DebugWindow::DebugWindow(Pipeline* pipeline)
    : dlib::drawable_window(false, false),
      pause_button(*this),
      paused(false),
      pipeline(pipeline),
      pipeline_steps(pipeline->get_steps()),
      pipeline_tabs(*this),
      statistics_widget(*this) {
  this->set_size(this->w_width, this->w_height);
  this->set_title("GazeTracker DebugWindow");

  this->pause_button.set_pos(this->w_margin, this->w_margin);
  this->pause_button.set_name("Pause");
  this->pause_button.set_click_handler([this]() {
        this->paused ^= true;
  });

  this->statistics_widget.set_pos(
      this->w_margin,
      this->pause_button.bottom() + this->w_margin);
  this->statistics_widget.set_size(
      this->stat_width,
      this->w_height - 3 * this->w_margin - this->pause_button.height());
  this->statistics_widget.set_grid_size(this->pipeline_steps.size() + 1, 2);
  this->statistics_widget.set_text(0, 0, "Pipeline step");
  this->statistics_widget.set_text(0, 1,
      dlib::convert_utf8_to_utf32("Execution time (\u00B5s)"));
  for (auto row = decltype(this->statistics_widget.number_of_rows()){0};
       row < this->statistics_widget.number_of_rows();
       ++row) {
    for (auto col = decltype(this->statistics_widget.number_of_columns()){0};
         col < this->statistics_widget.number_of_columns();
         ++col) {
      this->statistics_widget.set_editable(row, col, false);
    }
  }

  this->pipeline_tabs.set_pos(this->statistics_widget.right() + this->w_margin,
                              this->pause_button.bottom() + this->w_margin);
  this->pipeline_tabs.set_size(this->w_width - 3 * this->w_margin
                                   - this->stat_width,
                               this->w_height - 3 * this->w_margin
                                   - this->pause_button.height());
  this->pipeline_tabs.set_number_of_tabs(
      this->pipeline_steps.size() > 0 ? this->pipeline_steps.size() : 1);
  this->pipeline_tabs.set_click_handler([this](auto new_idx, auto)
      -> void {
        this->process_data(new_idx);
      });

  int width = this->pipeline_tabs.width() - 3 * this->w_margin;
  int height = this->pipeline_tabs.height() - 3 * this->w_margin
                  - this->pause_button.height();
  for (auto i = decltype(this->pipeline_steps.size()){0};
       i < this->pipeline_steps.size(); ++i) {
    std::shared_ptr<dlib::drawable> widget;
    if (VisualizeableBase* step =
        dynamic_cast<VisualizeableBase*>(this->pipeline_steps[i])) {
      widget = step->init(*this, width, height);
    } else {
      std::shared_ptr<dlib::label> tmp(new dlib::label(*this));
      tmp->set_text("No visualization.");
      widget = tmp;
    }

    // Create widget group, add widget to it
    std::shared_ptr<dlib::widget_group> widget_group_ptr(
        new dlib::widget_group(*this));
    this->widgets.push_back(widget_group_ptr);
    widget_group_ptr->add(*widget.get(), this->w_margin, this->w_margin);

    // Assign name and add widget group to tab
    this->pipeline_tabs.set_tab_name(i, this->pipeline_steps[i]->get_name());
    this->pipeline_tabs.set_tab_group(i, *widget_group_ptr.get());

    // Assign name to statistics widget
    this->statistics_widget.set_text(i + 1, 0,
        this->pipeline_steps[i]->get_name());
  }

  this->show();
  if (this->pipeline_steps.size() > 0) {
    EventManager::instance().subscribe(this);
  }
}

DebugWindow::~DebugWindow() {
  EventManager::instance().unsubscribe(this);
  for (PipelineStep* step : this->pipeline_steps) {
    if (VisualizeableBase* vb = dynamic_cast<VisualizeableBase*>(step)) {
      if (vb) {
        vb->remove_widget();
      }
    }
  }
  this->close_window();
}

void DebugWindow::process_data(int idx) {
  idx = idx < 0 ? this->pipeline_tabs.selected_tab() : idx;

  if (this->pipeline_steps.size() == 0) {
    return;
  }

  if (VisualizeableBase* step =
      dynamic_cast<VisualizeableBase*>(this->pipeline_steps[idx])) {
    step->visualize(this->data);
  }

  for (auto i = decltype(this->pipeline_steps.size()){1};
       i <= this->pipeline_steps.size(); ++i) {
    std::ostringstream os;
    os << this->data.execution_times[this->pipeline_steps[i - 1]->get_name()];
    this->statistics_widget.set_text(i, 1, os.str());
  }
}

void DebugWindow::on_user_event(void*, int event_type) {
  switch (event_type) {
    case Events::PIPELINE_DATA_UPDATED:
      if (this->paused) {
        break;
      }
      this->data = this->pipeline->get_data();
      this->process_data();
      break;
  }
}

namespace {

/**
 * Actual debug window routine to be called by open_debug_window().
 * It creates an instance of a DebugWindow and starts its main loop.
 *
 * @param pipeline the Pipeline for the DebugWindow.
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
