#ifndef INCLUDE_GAZE_PIPELINE_STEPS_GAZE_POINT_CALCULATION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_GAZE_POINT_CALCULATION_H_

#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace util {

/**
 * Parses a string representing an aspect ratio.
 * If the string contains : or /, it is split there and the aspect ratio is the
 * quotient between the values left and right:
 * `a:b` is equal to `a/b`.
 * If non of the tokens is present, the string is parsed as a double.
 *
 * @param aspect_ratio_string The string to parse.
 * @returns the parsed aspect ratio.
 */
double parse_aspect_ratio(std::string aspect_ratio_string);

/**
 * If value is smaller than min, min is returned.
 * If value is bigger than max, max is returned.
 * Else, the value is returned.
 *
 * @param value The value to clamp.
 * @param min The minimum allowed value.
 * @param max The maximum allowed value.
 *
 * @return The clamped value.
 */
double clamp(double value, double min, double max);

}  // namespace util

namespace pipeline {

/**
 * @class GazePointCalculation gaze_point_calculation.h "gaze/pipeline_steps/gaze_point_calculation.h"
 * @brief Calculates the gaze point.
 *
 * // TODO(shoeffner): Write documentation for GazePointCalculation.
 */
class GazePointCalculation final
    : public PipelineStep,
      public gui::PerspectiveVisualizeable {
  double focus_height;
  double focus_width;
  double sensor_height;
  double sensor_width;
  double pixel_height;
  double pixel_width;
  cv::Matx33d camera_matrix;
  cv::Mat distortion_coefficients;
  std::vector<cv::Vec3d> eye_ball_centers;
  std::vector<cv::Vec3d> model;
  std::vector<decltype(util::Data::landmarks.num_parts())> landmark_indices;
  double screen_width_m;
  double screen_height_m;
  double screen_width_px;
  double screen_height_px;
  double camera_offset_x;
  double camera_offset_y;

  /**
   * Calculates the distance between the model and its image given
   * two image points and an expected model distance between them.
   *
   * @note This method does *not* take distortions into account.
   *
   * @param p0 first point
   * @param p1 first point
   * @param expected_model_distance Distance between p0 and p1 in the model.
   * @return An estimate between the model and the camera. This is only
   *         accurate for unrotated and undistorted projections.
   */
  double calculate_distance(const dlib::point& p0, const dlib::point& p1,
      double expected_model_distance);

  /**
   * Calculates the estimated affine transformation from the projected
   * landmarks to the original 3D model.
   *
   * @param landmarks the landmarks with z = estimated distance
   *
   * @return An estimated transformation calculated by cv::estimateAffine3D
   */
  cv::Matx34d invertProjection(const std::vector<cv::Vec3d>& landmarks);

  /**
   * This is a proper inverse function to cv::projectPoints.
   * It undistorts the points, rescales them and reverts the estimated
   * translation and rotation. It restores points up to each point's
   * offset between the model and a plane.
   *
   * @param points The points to unproject.
   * @param translation The translation to revert.
   * @param rotation The rotation to revert.
   * @param distance The assumed distance to rescale the points.
   *
   * @return unprojected points.
   */
  std::vector<cv::Vec3d> unprojectPoints(
      const std::vector<cv::Vec2d>& points, const cv::Vec3d& translation,
      const cv::Matx33d& rotation, double distance);

  /**
   * Calculates the direction vector which points from the model origin
   * to the camera.
   *
   * @param data The data object
   * @param translation The head translation vector
   * @param rotation The head rotation matrix from cv::Rodrigues()
   * @param distance The distance from calculate_distance()
   *
   * @return The normal pointing from the model towards the screen.
   */
  cv::Vec3d get_model_to_camera_dir(
      const util::Data& data, const cv::Vec3d& translation,
      const cv::Matx33d& rotation, double distance);

  /**
   * Calculates the camera position in model coordinates.
   *
   * @param model_to_camera_dir Result of get_model_to_camera_dir()
   * @param distance Distance between model and camera.
   *
   * @return (0, 0, 0) + distance * model_to_camera_dir
   */
  cv::Vec3d get_camera_pos(const cv::Vec3d& get_model_to_camera_dir,
      double distance);

  /**
   * Returns the four estimated screen corners.
   *
   * @param camera_pos The camera position
   * @param translation The head translation vector
   * @param rotation The head rotation matrix from cv::Rodrigues()
   * @param distance The distance from calculate_distance()
   *
   * @return a vector containing the four screen corners in the order:
   *         top left, rop right, bottom right, bottom left
   */
  std::vector<cv::Vec3d> get_screen_corners(
      const cv::Vec3d& camera_pos, const cv::Vec3d& translation,
      const cv::Matx33d& rotation, double distance);

  /**
   * Calculates the intersection between the line going through
   * eye_ball_center and pupil and the screen plane.
   *
   * The three screen points should describe the plane (i.e. don't lie in one
   * line).
   *
   * This method solves the following system (see
   * <a href="https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection#Parametric_form">wikipedia for details</a>):
   * @f[
   * \left(\begin{array}{c}
   * t \\
   * u \\
   * v
   * \end{array}\right)
   * =
   * \left(\begin{array}{ccc}
   * e_x - p_x & b_x - a_x & c_x - a_x \\
   * e_y - p_y & b_y - a_y & c_y - a_y \\
   * e_z - p_z & b_z - a_z & c_z - a_z
   * \end{array}\right)^{-1}
   * \left(\begin{array}{c}
   * e_x - a_x \\
   * e_y - a_y \\
   * e_z - a_z
   * \end{array}\right),
   * @f]
   * with @f$a, b, c@f$ being the screen corners, @f$e, p@f$ the eye ball
   * center and pupil, all in model coordinates.
   *
   * @param eye_ball_center The eye ball center
   * @param pupil The pupil
   * @param screen_a one screen point
   * @param screen_b one screen point
   * @param screen_c one screen point
   *
   * @returns A matrix containing two column vectors:
   *          The first column is the 3D gaze point, the second
   *          column contains the scalars t, u, v.
   */
  cv::Matx32d calculate_gaze_point(
      const cv::Vec3d& eye_ball_center, const cv::Vec3d& pupil,
      const cv::Vec3d& screen_a, const cv::Vec3d& screen_b,
      const cv::Vec3d& screen_c);

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
