// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_SPSC_DEQUE_H_
#define INCLUDE_GAZE_UTIL_SPSC_DEQUE_H_

#include <deque>
#include <mutex>  // NOLINT
#include <shared_mutex>  // NOLINT


namespace gaze {

namespace util {

/**
 * This implements a single producer single consumer thread-safe deque.
 *
 * There can be multiple "listeners" getting the front or back items,
 * but as soon as one of them consumes them using SPSCDeque::pop_front() or
 * SPSCDeque::pop_back(), those items are no longer available.
 *
 * Thread-safety is guaranteed using std::unique_lock for write and
 * std::shared_lock for read accesses.
 */
template<typename T> class SPSCDeque {
  std::deque<T> _deque;
  std::shared_mutex _mutex;

 public:
    const void push_back(const T elem) {
      std::unique_lock<std::shared_mutex> lock(this->_mutex);
      this->_deque.push_back(elem);
    }

    const void push_front(const T elem) {
      std::unique_lock<std::shared_mutex> lock(this->_mutex);
      this->_deque.push_front(elem);
    }

    const bool empty() {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      return this->_deque.empty();
    }

    const T front() {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      return this->_deque.front();
    }

    const T back() {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      return this->_deque.back();
    }

    const T front_or_default(T _default) {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      if (!this->_deque.empty()) {
        return this->_deque.front();
      }
      return _default;
    }

    const T back_or_default(T _default) {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      if (!this->_deque.empty()) {
        return this->_deque.back();
      }
      return _default;
    }

    const T pop_front() {
      std::unique_lock<std::shared_mutex> lock(this->_mutex);
      T elem = this->_deque.front();
      this->_deque.pop_front();
      return elem;
    }

    const T pop_back() {
      std::unique_lock<std::shared_mutex> lock(this->_mutex);
      T elem = this->_deque.back();
      this->_deque.pop_back();
      return elem;
    }

    const int size() {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      return this->_deque.size();
    }
};

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_SPSC_DEQUE_H_
