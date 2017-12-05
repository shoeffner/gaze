#ifndef INCLUDE_GAZE_PIPELINE_STEPS_GAZE_POINT_CALCULATION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_GAZE_POINT_CALCULATION_H_

#include "gaze/gui/visualizeable.h"
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
class GazePointCalculation final
    : public PipelineStep,
      public gui::LabelVisualizeable {
  double focus_height;
  double focus_width;
  double sensor_height;
  double sensor_width;
  double pixel_height;
  double pixel_width;

 protected:
  /**
   * Calculates the sensor width and sensor height from the sensor
   * diagonal and its aspect ratio.
   */
  void set_sensor_size(double sensor_diagonal, double aspect_ratio);

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
