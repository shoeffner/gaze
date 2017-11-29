#ifndef INCLUDE_GAZE_PIPELINE_STEPS_HEAD_POSE_ESTIMATION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_HEAD_POSE_ESTIMATION_H_

#include <vector>

#include "dlib/gui_widgets.h"
#include "opencv2/opencv.hpp"

#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class HeadPoseEstimation head_pose_estimation.h "gaze/pipeline_steps/head_pose_estimation.h"
 * @brief Estimates the head pose from landmarks and source image.
 *
 * // TODO(shoeffner): Write documentation for HeadPoseEstimation.
 */
class HeadPoseEstimation final
    : public PipelineStep,
      public gui::ImageVisualizeable {
  std::vector<int> index_list;
  std::vector<cv::Point3d> model_points;
  double model_scale = 1;
  std::vector<dlib::image_display::overlay_line> coord_base_overlay;

  void update_overlay(const util::Data& data);

 public:
  HeadPoseEstimation();

  /**
   * Estimates the head pose from face landmarks and the input image.
   *
   * @param data The data object to be updated.
   * @returns via `data` the modified data object.
   */
  void process(util::Data& data) override;

  /**
   * Visualizes the estimated head pose.
   *
   * @param data The data object to be visualized.
   */
  void visualize(util::Data& data) override;
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_HEAD_POSE_ESTIMATION_H_
