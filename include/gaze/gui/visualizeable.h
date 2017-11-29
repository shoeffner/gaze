#ifndef INCLUDE_GAZE_GUI_VISUALIZEABLE_H_
#define INCLUDE_GAZE_GUI_VISUALIZEABLE_H_

#include <memory>
#include <string>

#include "dlib/gui_widgets.h"

#include "gaze/util/data.h"


namespace gaze {

/**
 * @namespace gui
 * @brief Contains everything related to debug visualizations.
 */
namespace gui {

/**
 * @namespace traits
 * @brief Defines traits for dlib::widget instances.
 *
 * Defines traits so that composition using visualization components becomes
 * possible.
 */
namespace traits {

//@{ Trait tags
/**
 * @struct sizeable
 * @brief A tag for resizeable widgets.
 */
struct sizeable {};

/**
 * @struct textable
 * @brief A tag for widgets which contain text messages.
 */
struct textable {};
//@}

//@{ Traits
/**
 * @struct widget_trait
 * @brief Template to assign tags to widgets as a trait.
 *
 * @tparam widget_type a dlib widget type.
 */
template<typename widget_type>
struct widget_trait;

/**
 * @struct widget_trait<dlib::label>
 * @brief Specialization of widget_trait for dlib::label.
 *
 * Adds the textable trait.
 */
template<> struct widget_trait<dlib::label> {
  /**
   * The trait tag.
   */
  typedef textable tag;
};

/**
 * @struct widget_trait<dlib::image_display>
 * @brief Specialization of widget_trait for dlib::image_display.
 *
 * Adds the sizeable trait.
 */
template<> struct widget_trait<dlib::image_display> {
  /**
   * The trait tag.
   */
  typedef sizeable tag;
};

/**
 * @struct widget_trait<dlib::perspective_display>
 * @brief Specialization of widget_trait for dlib::perspective_display.
 *
 * Adds the sizeable trait.
 */
template<> struct widget_trait<dlib::perspective_display> {
  /**
   * The trait tag.
   */
  typedef sizeable tag;
};
//@}

}  // namespace traits

//@{ adjust_widget
/**
 * For widgets with a sizeable trait, this function adjusts their size as
 * specified.
 *
 * @param widget The widget to adjust.
 * @param width The widget width.
 * @param height The widget height.
 * @param unused string.
 * @param unused trait, determines which adjust_widget is called.
 * @tparam widget_type a dlib widget type.
 */
template<typename widget_type>
void adjust_widget(std::shared_ptr<widget_type> widget,
    int width, int height, std::string, traits::sizeable) {
  widget->set_size(width, height);
}

/**
 * For widgets with a textable trait, this function adjusts their text as
 * specified.
 *
 * @param widget The widget to adjust.
 * @param unused int
 * @param unused int
 * @param text The text to set.
 * @param unused trait, determines which adjust_widget is called.
 * @tparam widget_type a dlib widget type.
 */
template<typename widget_type>
void adjust_widget(std::shared_ptr<widget_type> widget,
    int, int, std::string text, traits::textable) {
  widget->set_text(text);
}

/**
 * Calls the adjust_widget function for a widget's trait.
 *
 * @param widget The widget to adjust.
 * @param width The widget width.
 * @param height The widget height.
 * @param text The test to set.
 * @tparam widget_type a dlib widget type.
 */
template<typename widget_type>
void adjust_widget(std::shared_ptr<widget_type> widget,
    int width, int height, std::string text) {
  adjust_widget(widget, width, height, text,
      typename traits::widget_trait<widget_type>::tag());
}
//@}

/**
 * Creates a new std::shared_ptr for a widget.
 *
 * @param parent The parent window.
 * @tparam widget_type a dlib widget type.
 *
 * @returns A shared pointer to the new widget.
 */
template<typename widget_type>
// NOLINTNEXTLINE
std::shared_ptr<widget_type> create_widget(dlib::drawable_window& parent) {
  return std::shared_ptr<widget_type>(new widget_type(parent));
}

/**
 * @class VisualizeableBase visualizeable.h "gaze/gui/visualizeable.h"
 * @brief Abstract base class for Visualizeable to allow generic pointers.
 *
 * This class defines an interface which is readily implemented by
 * Visualizeable. Since Visualizeable is a template class, generic pointers
 * and collections containing different Visualizeables must instead use
 * VisualizeableBase.
 */
class VisualizeableBase {
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
   *
   * @returns A pointer to the newly initialized widget.
   */
  virtual std::shared_ptr<dlib::drawable> init(
      dlib::drawable_window& parent,  // NOLINT
      int width, int height,
      std::string text = "No visualization.") = 0;

  /**
   * Removes the widget or disables it.
   */
  virtual void remove_widget() {}

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

/**
 * @class Visualizeable visualizeable.h "gaze/gui/visualizeable.h"
 * @brief Implements VisualizeableBase for several widget types.
 *
 * This is a generic implementation for visualizeables using different widget
 * types.
 *
 * @tparam widget_type a dlib widget type.
 */
template<typename widget_type = dlib::label>
class Visualizeable : public VisualizeableBase {
 protected:
  /**
   * The widget to write to.
   */
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

  /**
   * Disables and removes the widget.
   */
  void remove_widget() override {
    if (this->widget && this->widget.get()) {
      this->widget->disable();
    }
    this->widget = std::shared_ptr<widget_type>(nullptr);
  }
};

/**
 * A Visualizeable with a dlib::label widget.
 */
typedef Visualizeable<dlib::label> LabelVisualizeable;

/**
 * A Visualizeable with a dlib::image_display widget.
 */
typedef Visualizeable<dlib::image_display> ImageVisualizeable;

/**
 * A Visualizeable with a dlib::perspective_display widget.
 */
typedef Visualizeable<dlib::perspective_display> PerspectiveVisualizeable;

}  // namespace gui

}  // namespace gaze

#endif  // INCLUDE_GAZE_GUI_VISUALIZEABLE_H_
