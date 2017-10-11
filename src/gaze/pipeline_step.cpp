// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_step.h"

#include <chrono>  // NOLINT
#include <thread>  // NOLINT

#include "gaze/util/data.h"
#include "gaze/util/spsc_deque.h"


namespace gaze {

const void PipelineStep::start() {
  this->running = true;
}

const void PipelineStep::stop() {
  this->running = false;
}

util::Data PipelineStep::get_data(
    util::SPSCDeque<util::Data>* const in_deque) const {
  while (in_deque->empty()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
  return in_deque->pop_front();
}

const void PipelineStep::store_data(
    util::SPSCDeque<util::Data>* const out_deque,
    const util::Data data) const {
  out_deque->push_back(data);
}

const void PipelineStep::operator()(
    util::SPSCDeque<util::Data>* const in_deque,
    util::SPSCDeque<util::Data>* const out_deque) {
  // Wait for the starting signal
  while (!this->running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  while (this->running) {
     // TODO(shoeffner): add e.g. timing instructions etc.
     util::Data data = this->get_data(in_deque);

     process(&data);

     // TODO(shoeffner): add e.g. timing results to data object.

     this->store_data(out_deque, data);
  }
}

}  // namespace gaze
