// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_GUI_PIPELINE_STEP_WIDGET_H_
#define INCLUDE_GAZE_GUI_PIPELINE_STEP_WIDGET_H_

#include <algorithm>

#include "dlib/gui_core.h"
#include "dlib/gui_widgets.h"

#include "gaze/util/utility.h"


namespace gaze {

namespace gui {

class PipelineStepWidget : public dlib::drawable {
  dlib::array2d<dlib::rgb_alpha_pixel> image;
  dlib::rectangle max_size;

 public:
    explicit PipelineStepWidget(dlib::drawable_window& window);
    ~PipelineStepWidget() override;

    void draw(const dlib::canvas& canvas) const override;

    void set_size(unsigned long width, unsigned long height);

    template<typename image_type>
    void set_image(const image_type& new_image) {
      dlib::auto_mutex M(this->m);
      dlib::assign_image(this->image, new_image);
      dlib::rectangle old(this->rect);

      // Calculate new size so that the image fits
      double conv_ratio = std::min(static_cast<double>(this->rect.width())
                                     / this->image.nc(),
                                   static_cast<double>(this->rect.height())
                                     / this->image.nr());
      this->rect = dlib::rectangle(
          this->rect.left(),
          this->rect.top(),
          this->rect.left() +
            static_cast<long>(this->image.nc() * conv_ratio + 0.5) - 1,
          this->rect.top() +
            static_cast<long>(this->image.nr() * conv_ratio + 0.5) - 1);
      this->parent.invalidate_rectangle(this->rect + old);
    }
};

}  // namespace gui

}  // namespace gaze

#endif  // INCLUDE_GAZE_GUI_PIPELINE_STEP_WIDGET_H_
