// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEP_H_
#define INCLUDE_GAZE_PIPELINE_STEP_H_

#include <atomic>

#include "gaze/util/data.h"
#include "gaze/util/spsc_deque.h"


namespace gaze {

/**
 * @class PipelineStep pipeline_step.h "gaze/pipeline_step.h"
 * @brief Abstract base class for PipelineSteps. Must be inherited from.
 *
 * When initializing a gaze::Pipeline, each step is put into its own thread.
 * They can communicate via an input and an output deque.
 *
 * Custom PipelineSteps must implement their own process(util::Data* data)
 * method. They should only add data to fields which were not written before.
 *
 * Custom PipelineSteps may implement their own get_data() and store_data()
 * functions. This is especially important for the first and last step in
 * a Pipeline: The first should ignore its input deque and create data objects
 * instead (inside the get_data() method), the last step should not store
 * its results to a final deque, but instead write to disk or similar. This may
 * be different if there are steps outside of the pipeline which provide data
 * or store it.
 */
class PipelineStep {
    std::atomic<bool> running = std::atomic<bool>(false);

 protected:
    /** @name Data flow
     * These methods may be optionally overwritten to customize the pipeline's
     * dataflow.
     */
    //@{
    /**
     * Retrieves a data object from in_deque and removes it from the deque.
     * If the in_deque is known to never be written to, this may be overwritten
     * by a factory method.
     *
     * @note If the deque is currently empty, the thread will block until the
     * deque contains data (in which case the data is consumed) or the thread
     * should stop.
     *
     * @param in_deque The deque to read data from.
     * @returns the first element of the in_deque, or implementation dependent
     *          new objects.
     */
    virtual util::Data get_data(
        util::SPSCDeque<util::Data>* const in_deque) const;

    /**
     * Stores the data after the call to process() into the out_deque for
     * further steps.
     *
     * gaze::pipeline::IntoTheVoidStep overwrites this method to not do
     * anything, allowing it to be used as a cleanup at the end of the
     * Pipeline.
     *
     * @param out_deque The deque to write to.
     * @param data The data object to write.
     */
    virtual const void store_data(
        util::SPSCDeque<util::Data>* const out_deque,
        const util::Data data) const;
    //@}

 public:
    virtual ~PipelineStep() = default;

    /**
     * This function must be overwritten by each PipelineStep.
     * It should process the data object and write back its own results.
     * While it is not forbidden to overwrite values, this should be done with
     * caution.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    virtual void process(util::Data* data) = 0;

    //@{
    /**
     * Starts the thread's processing. Until this function is called, threads
     * sit idle.
     */
    const void start();

    /**
     * Signals the thread to stop processing input data.
     * // TODO(shoeffner): Should steps finish processing of remaining items?
     */
    const void stop();
    //@}

    /**
     * The operator() makes this class a functor for threads.
     *
     * It handles calling get_data() on the in_deque, the custom process()
     * method and the call to store_data().
     *
     * @note This function should not be called by custom code.
     *
     * @param in_deque The input deque. This is the previous step's output.
     * @param out_deque The output deque. This is the following step's input.
     */
    const void operator()(
        util::SPSCDeque<util::Data>* const in_deque,
        util::SPSCDeque<util::Data>* const out_deque);
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEP_H_
