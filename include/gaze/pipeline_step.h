// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEP_H_
#define INCLUDE_GAZE_PIPELINE_STEP_H_

#include <atomic>
#include <string>

#include "opencv2/core.hpp"

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
  static int counter;
  int number;

 public:
    /**
     * Increments the PipelineStep counter and assigns a position for potential
     * debug texts.
     */
    PipelineStep();

    /** @name Inheriting from PipelineStep
     *
     * // TODO(shoeffner): Explain in more detail how to inherit from
     *                     PipelineStep.
     */
    //@{
    /**
     * Overwrite this method if you need more destruction control.
     */
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
    virtual void process(util::Data& data) = 0;

    /**
     * Modifies the source image to visualize the results of this step.
     * This method may be implemented by subclasses.
     *
     * @param data The data object to be updated.
     */
    virtual void visualize(util::Data& data);
    //@}
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEP_H_
