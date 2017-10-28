// Copyright 2017 Sebastian Höffner

#include "gaze/gui/pipeline_step_widget.h"


namespace gaze {

namespace gui {

PipelineStepWidget::PipelineStepWidget(dlib::drawable_window& window)
    : drawable(window), max_size(640, 480) {
  this->enable_events();
}

PipelineStepWidget::~PipelineStepWidget() {
  this->disable_events();
  this->parent.invalidate_rectangle(this->rect);
}

void PipelineStepWidget::draw(const dlib::canvas& canvas) const {
  dlib::rectangle area = this->rect.intersect(canvas);
  if (area.is_empty()) {
    return;
  }
  dlib::draw_image(canvas, this->rect, this->image);
}

void PipelineStepWidget::set_size(unsigned long width, unsigned long height) {
  dlib::auto_mutex M(this->m);
  dlib::rectangle old(this->rect);
  this->rect = dlib::resize_rect(this->rect, width, height);
  this->max_size = dlib::rectangle(0, 0, width, height);
  this->parent.invalidate_rectangle(this->rect + old);
}


}  // namespace gui

}  // namespace gaze
