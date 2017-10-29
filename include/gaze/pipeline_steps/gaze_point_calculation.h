// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_GAZE_POINT_CALCULATION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_GAZE_POINT_CALCULATION_H_

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class GazePointCalculation gaze_point_calculation.h "gaze/pipeline_steps/gaze_point_calculation.h"
 * @brief Calculates the gaze point.
 *
 * // TODO(shoeffner): Write documentation for GazePointCalculation.
 */
class GazePointCalculation final : public PipelineStep {
 public:
    GazePointCalculation();

    /**
     * Calculates the gaze point given a head pose and eye centers.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    void process(util::Data& data) override;

    /**
     * Visualizes the estimated gaze point.
     *
     * @param data The data object to be visualized.
     */
    void visualize(util::Data& data) override;
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_GAZE_POINT_CALCULATION_H_
