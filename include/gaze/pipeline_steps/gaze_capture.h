#ifndef INCLUDE_GAZE_PIPELINE_STEPS_GAZE_CAPTURE_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_GAZE_CAPTURE_H_

#ifdef HAS_CAFFE
  #include "itracker/itracker.h"

  #include "opencv2/opencv.hpp"
#endif

#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class GazeCapture gaze_capture.h "gaze/pipeline_steps/gaze_capture.h"
 * @brief Uses iTracker to track Gaze.
 *
 * This step does nothing if not compiled using the -DWITH_CAFFE=ON option.
 *
 * If it is compiled, it holds a pointer to an instance of itracker::ITracker,
 * which it passes extracted eyes, the face, and a face mask to. It then maps
 * the result to the target area on the screen.
 */
class GazeCapture final
    : public PipelineStep,
      public gui::ImageVisualizeable {
  #ifdef HAS_CAFFE
    std::unique_ptr<itracker::ITracker> itracker;

    cv::Vec2d camera_offset;
    cv::Vec2d screen_size_m;
    cv::Vec2d target_size;
  #endif

 public:
  GazeCapture();

  /**
   * Estimates the gaze point using iTracker.
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

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_GAZE_CAPTURE_H_

