// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_INTO_THE_VOID_STEP_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_INTO_THE_VOID_STEP_H_

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"
#include "gaze/util/spsc_deque.h"

namespace gaze {

namespace pipeline {

/**
 * @class IntoTheVoidStep into_the_void_step.h
 *        "gaze/pipeline_steps/into_the_void_step.h"
 * @brief Cleans up at the end of the pipeline.
 *
 * This step pops elements from its input deque but never performs any more
 * operations (other than timings etc.) on the data objects. Especially
 * important is that it does not append the data object to the output deque. It
 * thus avoids memory from filling up.
 *
 * @note This step is automatically added at the end of the Pipeline.
 */
class IntoTheVoidStep : public PipelineStep {
 protected:
    /**
     * Does nothing. And that's why this class is here.
     *
     * By doing nothing, this removes data from the pipeline, avoiding out of
     * memory errors.
     *
     * @param out_deque The deque to write to. Ignored.
     * @param data The data object to write. Ignored.
     */
    virtual const void store_data(
        util::SPSCDeque<util::Data>* const out_deque,
        const util::Data data) const;

 public:
    /**
     * Does nothing.
     *
     * @param data The data object to be updated. Ignored.
     */
    virtual void process(util::Data* data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_INTO_THE_VOID_STEP_H_
