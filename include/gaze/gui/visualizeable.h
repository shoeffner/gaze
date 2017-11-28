#ifndef INCLUDE_GAZE_GUI_VISUALIZEABLE_H_
#define INCLUDE_GAZE_GUI_VISUALIZEABLE_H_

#include <memory>
#include <string>

#include "dlib/gui_widgets.h"

#include "gaze/util/data.h"


namespace gaze {

namespace gui {

namespace traits {

struct sizeable {};
struct textable {};

template<typename widget_type>
struct widget_trait;

template<> struct widget_trait<dlib::label> {
  typedef textable tag;
};

template<> struct widget_trait<dlib::image_display> {
  typedef sizeable tag;
};

template<> struct widget_trait<dlib::perspective_display> {
  typedef sizeable tag;
};

}  // namespace traits

template<typename widget_type>
void adjust_widget(std::shared_ptr<widget_type> widget,
    int width, int height, std::string, traits::sizeable) {
  widget->set_size(width, height);
}

template<typename widget_type>
void adjust_widget(std::shared_ptr<widget_type> widget,
    int, int, std::string text, traits::textable) {
  widget->set_text(text);
}

template<typename widget_type>
void adjust_widget(std::shared_ptr<widget_type> widget,
    int width, int height, std::string text) {
  adjust_widget(widget, width, height, text,
      typename traits::widget_trait<widget_type>::tag());
}

template<typename widget_type>
// NOLINTNEXTLINE
std::shared_ptr<widget_type> create_widget(dlib::drawable_window& parent) {
  return std::shared_ptr<widget_type>(new widget_type(parent));
}

class VisualizeableBase {
 protected:
  std::shared_ptr<dlib::drawable> widget;

 public:
  /**
   * Override this method if you need more destruction control.
   */
  virtual ~VisualizeableBase() = default;

  /**
   * Initializes the widget by creating a widget and adjusting it.
   *
   * @note Some widgets do not support all adjustments (width/height/text).
   *       Please refer to the widget documentations for details.
   *
   * @param parent The parent window.
   * @param width The widget width
   * @param height The widget height
   * @param text The widget text
   */
  virtual std::shared_ptr<dlib::drawable> init(
      dlib::drawable_window& parent,  // NOLINT
      int width, int height,
      std::string text = "No visualization.") = 0;

  /**
   * Removes the widget by setting its pointer to the nullptr.
   */
  void remove_widget() {
    if (this->widget && this->widget.get()) {
      this->widget->disable();
    }
  }

  /**
   * Visualizes the data object by updating the widget.
   *
   * The data object is not const to allow for certain non-const actions,
   * but it should be highly discouraged to modify it.
   *
   * @warning Always check if this->widget is not null before writing to it!
   *
   * @param data The data object to be visualized.
   */
  virtual void visualize(util::Data& data) = 0;  // NOLINT
};

template<typename widget_type = dlib::label>
class Visualizeable : public VisualizeableBase {
 protected:
  std::shared_ptr<widget_type> widget;

 public:
  virtual ~Visualizeable() = default;

  std::shared_ptr<dlib::drawable> init(
      dlib::drawable_window& parent,  // NOLINT
      int width, int height,
      std::string text = "No visualization.") final {
    widget = create_widget<widget_type>(parent);
    adjust_widget<widget_type>(widget, width, height, text);
    return widget;
  }
};

typedef Visualizeable<dlib::label> LabelVisualizeable;
typedef Visualizeable<dlib::image_display> ImageVisualizeable;
typedef Visualizeable<dlib::perspective_display> PerspectiveVisualizeable;

}  // namespace gui

}  // namespace gaze

#endif  // INCLUDE_GAZE_GUI_VISUALIZEABLE_H_
