#include "gaze/pipeline.h"

#include <shared_mutex>  // NOLINT
#include <string>
#include <thread>  // NOLINT
#include <utility>
#include <vector>

#include "gaze/gui/event_manager.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

void Pipeline::operator()() {
  while (!this->running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
  while (this->running) {
    // Process
    util::Data data;
    for (PipelineStep* step : this->steps) {
      std::chrono::high_resolution_clock::time_point start =
        std::chrono::high_resolution_clock::now();

      step->process(data);

      std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();
      data.execution_times.insert(
          std::pair<std::string, double>(
            step->get_name(),
            std::chrono::duration_cast<std::chrono::microseconds>(
              end - start).count()));
    }
    {  // Update data (locked)
      std::unique_lock<std::shared_mutex> lock(this->data_access_mutex);
      this->current_data = data;
    }
    // Notify EventManager that the data was updated.
    gui::EventManager::instance().publish(nullptr,
        gui::Events::PIPELINE_DATA_UPDATED);
  }
}

Pipeline::Pipeline(std::vector<PipelineStep*> steps,
                   const bool start) : steps(steps) {
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

void Pipeline::start() {
  this->running = true;
}

void Pipeline::stop() {
  this->running = false;
}

util::Data Pipeline::get_data() {
  std::shared_lock<std::shared_mutex> lock(this->data_access_mutex);
  return this->current_data;
}

std::vector<PipelineStep*> Pipeline::get_steps() {
  return this->steps;
}

}  // namespace gaze
