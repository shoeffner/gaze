// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_GUI_EVENT_MANAGER_H_
#define INCLUDE_GAZE_GUI_EVENT_MANAGER_H_

#include <vector>

#include "dlib/gui_core.h"


namespace gaze {

namespace gui {

/**
 * @class Events event_manager.h "gaze/gui/event_manager.h"
 * @brief An enum of events to make it easy to distinguish them with names instead of numbers.
 *
 * @note This is no strongly typed enum (`enum class`) to be
 *       compatible with
 *       dlib::base_window::trigger_user_event(void*,int).
 */
enum Events : int {
  PIPELINE_DATA_UPDATED
};

/**
 * @class EventManager event_manager.h "gaze/gui/event_manager.h"
 * @brief Handles communication between non-gui and gui processes in a safe way.
 *
 * This EventManager performs no sophisticated distinction
 * about subcribers to specific events, from specific callers,
 * etc., instead it just passes **all** published events to
 * **all** subscribers.
 *
 * @note The EventManager is implemented as a singleton, it should be
 * accessed via EventManager::instance().
 */
class EventManager {
  std::vector<dlib::base_window*> subscribers;

  /**
   * Explicitly private constructor for singleton pattern.
   */
  EventManager();

 public:
  /**
   * @returns a reference to a global static instance of the
   * event manager.
   */
  static EventManager& instance();

  /**
   * Publishes an event with the given data to all subscribers
   * using dlib::base_window::trigger_user_event(void*,int).
   *
   * @param data the data to pass along
   * @param event the event identifier
   */
  void publish(void* data, Events event);

  /**
   * Subscribes the new subscriber to all events.
   *
   * @param subscriber The new subscriber.
   */
  void subscribe(dlib::base_window* subscriber);

  /**
   * Unsubscribes the subscriber from all events.
   *
   * @param subscriber The unsubscriber.
   */
  void unsubscribe(dlib::base_window* subscriber);
};

}  // namespace gui

}  // namespace gaze

#endif  // INCLUDE_GAZE_GUI_EVENT_MANAGER_H_
