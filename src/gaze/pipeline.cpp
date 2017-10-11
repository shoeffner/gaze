// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline.h"

#include <thread>  // NOLINT
#include <vector>

#include "gaze/pipeline_step.h"
#include "gaze/pipeline_steps/into_the_void_step.h"
#include "gaze/util/data.h"
#include "gaze/util/spsc_deque.h"


namespace gaze {

Pipeline::Pipeline(std::vector<PipelineStep*> steps, const bool start) {
  this->steps = steps;
  // Always make sure to include the cleanup step.
  this->steps.push_back(new pipeline::IntoTheVoidStep());

  util::SPSCDeque<util::Data>* deque = new util::SPSCDeque<util::Data>();
  deques.push_back(deque);
  for (PipelineStep* step : this->steps) {
    deque = new util::SPSCDeque<util::Data>();
    this->deques.push_back(deque);
  }

  int i = 0;
  for (PipelineStep* step : this->steps) {
    std::thread* thread = new std::thread(
        std::ref(*step),
        std::ref(this->deques[i]),
        std::ref(this->deques[i+1]));
    this->threads.push_back(thread);
    ++i;
  }

  if (start) {
    this->start();
  }
}

Pipeline::~Pipeline() {
  for (PipelineStep* step : this->steps) {
    step->stop();
  }
  for (std::thread* thread : this->threads) {
    thread->join();
    delete thread;
  }
  for (util::SPSCDeque<util::Data>* deque : this->deques) {
    delete deque;
  }
}

const void Pipeline::start() const {
  for (PipelineStep* step : this->steps) {
    step->start();
  }
}

}  // namespace gaze
