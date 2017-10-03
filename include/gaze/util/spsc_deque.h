// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_SPSC_DEQUE_H_
#define INCLUDE_GAZE_UTIL_SPSC_DEQUE_H_

#include <deque>
#include <mutex>  // NOLINT
#include <shared_mutex>  // NOLINT


namespace gaze {

namespace util {

/**
 * @class SPSCDeque spsc_deque.h "gaze/util/spsc_deque.h"
 * @brief This implements a single producer single consumer thread-safe deque
 * ("double ended queue").
 *
 * There can be multiple "listeners" getting the front or back items,
 * but as soon as one of them consumes them using SPSCDeque::pop_front() or
 * SPSCDeque::pop_back(), those items are no longer available.
 *
 * Thread-safety is guaranteed using std::unique_lock for write and
 * std::shared_lock for read accesses.
 *
 * It is really just a wrapper around std::deque using some locks for
 * thread-safety.
 */
template<typename T> class SPSCDeque {
  std::deque<T> _deque;
  std::shared_mutex _mutex;

 public:
    //@{
    /**
     * Appends an element to the deque.
     *
     * @param elem The element to append.
     */
    const void push_back(const T elem) {
      std::unique_lock<std::shared_mutex> lock(this->_mutex);
      this->_deque.push_back(elem);
    }

    /**
     * Prepends an elment to the deque.
     *
     * @param elem The element to prepend.
     */
    const void push_front(const T elem) {
      std::unique_lock<std::shared_mutex> lock(this->_mutex);
      this->_deque.push_front(elem);
    }
    //@}

    /**
     * Checks if the deque is empty.
     *
     * @returns `true` if the deque is empty.
     */
    const bool empty() {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      return this->_deque.empty();
    }

    //@{
    /**
     * @returns the first element of the deque.
     */
    const T front() {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      return this->_deque.front();
    }

    /**
     * @returns the last element of the deque.
     */
    const T back() {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      return this->_deque.back();
    }

    /**
     * @param _default the default value to return if the deque is empty.
     * @returns the first element of the deque, unless it is empty. Then, the
     *          provided default value is returned.
     */
    const T front_or_default(const T _default) {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      if (!this->_deque.empty()) {
        return this->_deque.front();
      }
      return _default;
    }

    /**
     * @param _default the default value to return if the deque is empty.
     * @returns the last element of the deque, unless it is empty. Then, the
     *          provided default value is returned.
     */
    const T back_or_default(const T _default) {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      if (!this->_deque.empty()) {
        return this->_deque.back();
      }
      return _default;
    }
    //@}

    //@{
    /**
     * Retrieves the first element of the queue and removes it from the queue.
     *
     * @returns The removed element.
     */
    const T pop_front() {
      std::unique_lock<std::shared_mutex> lock(this->_mutex);
      T elem = this->_deque.front();
      this->_deque.pop_front();
      return elem;
    }

    /**
     * Retrieves the last element of the queue and removes it from the queue.
     *
     * @returns The removed element.
     */
    const T pop_back() {
      std::unique_lock<std::shared_mutex> lock(this->_mutex);
      T elem = this->_deque.back();
      this->_deque.pop_back();
      return elem;
    }
    //@}

    /**
     * @returns the current number elements inside the queue.
     */
    const int size() {
      std::shared_lock<std::shared_mutex> lock(this->_mutex);
      return this->_deque.size();
    }
};

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_SPSC_DEQUE_H_
