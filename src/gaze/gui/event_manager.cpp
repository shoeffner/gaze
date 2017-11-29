#include "gaze/gui/event_manager.h"

#include <vector>

#include "dlib/gui_core.h"


namespace gaze {

namespace gui {

EventManager::EventManager() {
}

EventManager& EventManager::instance() {
  static EventManager event_manager;
  return event_manager;
}

void EventManager::publish(void* data, Events event) {
  for (dlib::base_window* subscriber : this->subscribers) {
    subscriber->trigger_user_event(data, event);
  }
}

void EventManager::subscribe(dlib::base_window* subscriber) {
  this->subscribers.push_back(subscriber);
}

void EventManager::unsubscribe(dlib::base_window* subscriber) {
  this->subscribers.erase(
      std::remove(this->subscribers.begin(),
                  this->subscribers.end(),
                  subscriber),
      this->subscribers.end());
}

}  // namespace gui

}  // namespace gaze


