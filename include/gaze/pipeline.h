// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_H_
#define INCLUDE_GAZE_PIPELINE_H_

#include <thread>  // NOLINT
#include <vector>

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"
#include "gaze/util/spsc_deque.h"


namespace gaze {

/**
 * @class Pipeline pipeline.h "gaze/pipeline.h"
 * @brief The processing pipeline from input to output.
 *
 * Controlls all threads and deques involved in the processing of the eye
 * tracking results.
 */
class Pipeline {
  std::vector<PipelineStep*> steps;
  std::vector<std::thread*> threads;
  std::vector<util::SPSCDeque<util::Data>*> deques;

 public:
    //{@
    /**
     * Initializes the pipeline and starts the processing once all steps are
     * thrown into threads properly.
     * For each step (in order) input/output deques are created. Each step gets
     * its preceding step's output deque as its input deque, thus all steps are
     * chained.
     *
     * The first input queue will always be empty, as such PipelineSteps which
     * are used first should implement their own PipelineStep::get_data
     * function to ignore the input deque and instead create the data objects
     * which are passed along the pipeline.
     * The last step is always a pipeline::IntoTheVoidStep and assure that
     * the data objects are cleaned up.
     *
     * The start can be deferred and manually invoked by a call to start().
     *
     * @param steps The PipelineSteps to use.
     * @param start if `true`, the processing will start automatically.
     */
    explicit Pipeline(std::vector<PipelineStep*> steps,
        const bool start = true);
    ~Pipeline();
    //@}

    /**
     * Starts all underlying threads.
     */
    const void start() const;
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_H_
