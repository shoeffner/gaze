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
  cv::Matx33d camera_matrix;
  bool has_camera_matrix = false;
  cv::Mat distortions;
  bool has_distortions = false;

  void update_overlay(const util::Data& data);

 protected:
  /**
   * Returns a camera matrix.
   *
   * If it was not loaded yet, it will be read from `gaze.yaml` (as created using
   * <a href="https://github.com/opencv/opencv/blob/fc9e031454fd456d09e15944c99a419e73d80661/samples/cpp/calibration.cpp">OpenCV's
   * calibration tool</a>, also check the gaze.default.yaml to see how to integrate it).
   *
   * If nothing can be found in the configuration, the following matrix is
   * calculated and returned:
   *
   * @f[
   * \left(
   * \begin{array}{ccc}
   * c & 0 & \frac{c}{2} \\
   * 0 & c & \frac{r}{2} \\
   * 0 & 0 & 1
   * \end{array}
   * \right)
   * @f]
   *
   * Where @f$c=\mbox{image columns}, r = \mbox{image rows}@f$.
   *
   * @param data The data to read from, if no yaml data is available.
   * @returns a camera matrix.
   */
  cv::Matx33d read_or_set_camera_matrix(const util::Data& data);

  /**
   * Returns a distortions matrix.
   *
   * If the distortions matrix is available from the configuration,
   * the configuration value is stored and used. Otherwise, it
   * is assumed to be a zero matrix.
   *
   * @param data The data to read from, if no yaml data is available.
   * @returns a distortion matrix.
   */
  cv::Mat get_and_maybe_read_distortions(const util::Data& data);

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
