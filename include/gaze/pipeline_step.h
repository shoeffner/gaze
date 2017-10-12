// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEP_H_
#define INCLUDE_GAZE_PIPELINE_STEP_H_

#include <atomic>

#include "gaze/util/data.h"


namespace gaze {

/**
 * @class PipelineStep pipeline_step.h "gaze/pipeline_step.h"
 * @brief Abstract base class for PipelineSteps. Must be inherited from.
 *
 * Custom PipelineSteps must implement their own process(util::Data* data)
 * method. They should only add data to fields which were not written before.
 */
class PipelineStep {
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
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEP_H_
