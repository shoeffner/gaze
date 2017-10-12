// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline.h"

#include <thread>  // NOLINT
#include <vector>

#include "gaze/pipeline_step.h"
#include "gaze/pipeline_steps/debug_view.h"
#include "gaze/util/data.h"

namespace gaze {

const void Pipeline::operator()() {
  while (!this->running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
  while (this->running) {
    util::Data data;
    for (PipelineStep* step : this->steps) {
      step->process(&data);
    }
    if (this->debug) {
      for (PipelineStep* step : this->steps) {
        step->visualize(&data);
      }
      this->debug_view->process(&data);
    }
  }
}

Pipeline::Pipeline(std::vector<PipelineStep*> steps,
                   const bool start,
                   const bool debug) {
  this->steps = steps;
  this->debug = debug;
  this->debug_view = new pipeline::DebugView();
  this->thread = new std::thread(&Pipeline::operator(), std::ref(*this));
  if (start) {
    this->start();
  }
}

Pipeline::~Pipeline() {
  this->stop();
  this->thread->join();
  delete this->thread;
}

const void Pipeline::start() {
  this->running = true;
}

const void Pipeline::stop() {
  this->running = false;
}

}  // namespace gaze
