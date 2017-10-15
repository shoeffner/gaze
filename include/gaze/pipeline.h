// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_H_
#define INCLUDE_GAZE_PIPELINE_H_

#include <atomic>
#include <shared_mutex>  // NOLINT
#include <thread>  // NOLINT
#include <vector>

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

/**
 * @class Pipeline pipeline.h "gaze/pipeline.h"
 * @brief The processing pipeline from input to output.
 */
class Pipeline {
  std::atomic<bool> debug = std::atomic<bool>(false);
  std::atomic<bool> running = std::atomic<bool>(false);
  std::shared_mutex data_access_mutex;
  util::Data current_data;
  std::vector<PipelineStep*> steps;
  std::thread* thread;

  /**
   * The thread function.
   * As long as this->running is true, this pipeline will process data.
   * It waits until start() is called before processing (either implicit
   * through Pipeline::Pipeline or explicit by calling start()).
   */
  void operator()();

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
    void start();

    /**
     * Stops the processing of the underlying thread.
     */
    void stop();
    //@}

    //@{
    /**
     * @returns the last updated data element.
     */
    util::Data get_data();

    /**
     * @returns The PipelineSteps.
     */
    std::vector<PipelineStep*> get_steps();
    //@}
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_H_
