// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_H_
#define INCLUDE_GAZE_PIPELINE_H_

#include <atomic>
#include <thread>  // NOLINT
#include <vector>

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

/**
 * @class Pipeline pipeline.h "gaze/pipeline.h"
 * @brief The processing pipeline from input to output.
 *
 * Controlls all threads and deques involved in the processing of the eye
 * tracking results.
 */
class Pipeline {
  std::atomic<bool> running = std::atomic<bool>(false);
  std::vector<PipelineStep*> steps;
  std::thread* thread;

  /**
   * The thread function.
   * As long as this->running is true, this pipeline will process data.
   * It waits until start() is called before processing (either implicit
   * through Pipeline::Pipeline or explicit by calling start()).
   */
  const void operator()();

 public:
    //{@
    /**
     * Initializes the pipeline and starts the processing.
     *
     * The start can be deferred and manually invoked by a call to start().
     *
     * @param steps The PipelineSteps to use.
     * @param start if `true`, the processing will start automatically.
     */
    explicit Pipeline(std::vector<PipelineStep*> steps,
        const bool start = true);
    /**
     * Stops processing if not already done, joins the thread and deletes it.
     */
    ~Pipeline();
    //@}

    //{@
    /**
     * Starts the processing of the underlying thread.
     */
    const void start();

    /**
     * Stops the processing of the underlying thread.
     */
    const void stop();
    //@}
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_H_
