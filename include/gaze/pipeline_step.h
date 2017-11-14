// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEP_H_
#define INCLUDE_GAZE_PIPELINE_STEP_H_

#include <string>

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

 protected:
  /**
   * The number of the pipeline step.
   */
  int number;

  /**
   * The name of this PipelineStep.
   * Should be unique if possible. By default "Step #" is assigned.
   */
  std::string name;

 public:
  /**
   * Increments the PipelineStep counter.
   */
  PipelineStep();

  /** @name Inheriting from PipelineStep
   *
   * // TODO(shoeffner): Explain in more detail how to inherit from
   *                     PipelineStep.
   */
  //@{
  /**
   * Override this method if you need more destruction control.
   */
  virtual ~PipelineStep() = default;

  /**
   * This function must be overwritten by each PipelineStep.
   * It should process the data object and write back its own results.
   * While it is not forbidden to override values, this should be done with
   * caution.
   *
   * @param data The data object to be updated.
   * @returns via `data` the modified data object.
   */
  virtual void process(util::Data& data) = 0;  // NOLINT
  //@}

  /**
   * Returns the name of this step. If this is not overridden,
   * names it `Step #` where `#` is a number starting at 0 and increasing
   * with each step.
   *
   * @returns This step's name.
   */
  std::string get_name();
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEP_H_
