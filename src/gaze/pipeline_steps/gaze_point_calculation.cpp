#include "gaze/pipeline_steps/gaze_point_calculation.h"

#include <cmath>
#include <string>

#include "dlib/gui_widgets.h"
#include "opencv2/opencv.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"
#include "gaze/util/dlibcv.h"


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
double parse_aspect_ratio(std::string aspect_ratio_string) {
  double aspect_ratio;
  int delim = aspect_ratio_string.find(":");
  if (delim < 0) {
    delim = aspect_ratio_string.find("/");
  }
  if (delim > 0) {
    double width_rate = std::stod(aspect_ratio_string.substr(0, delim));
    double height_rate = std::stod(aspect_ratio_string.substr(delim + 1));
    aspect_ratio = width_rate / height_rate;
  } else {
    aspect_ratio = std::stod(aspect_ratio_string);
  }
  return aspect_ratio;
}

/**
 * Calculates the eye ball center.
 *
 * Uses a radius of 0.012 m @cite Davson2017.
 *
 * @f$c = \left(\begin{array}{c}
 * \frac{\mathit{ex}_x + \mathit{en}_x}{2} \\
 * \mathit{ex}_y \\
 * \mathit{ex}_z - \sqrt{ r^2 - \left(\mathit{ex}_x - \frac{\mathit{ex}_x + \mathit{en}_x}{2}\right)^2 }
 * \end{array}\right)$@f
 *
 * @param ex The exocanthion.
 * @param en The endocanthion.
 * @returns c (see above)
 */
cv::Vec3d eye_ball_center(cv::Vec3d ex, cv::Vec3d en) {
  double r_squared = 0.000144;
  cv::Vec3d center;
  center[0] = (ex[0] + en[0]) / 2;
  center[1] = ex[1];

  double diff = ex[0] - center[0];
  center[2] = ex[2] - std::sqrt(r_squared - diff * diff);
  return center;
}

}  // namespace util

namespace pipeline {

GazePointCalculation::GazePointCalculation() {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "GazePoint";

  YAML::Node camera_config = util::get_config()["meta"]["camera"];
  double sensor_aspect_ratio = camera_config["sensor_aspect_ratio"] ?
    util::parse_aspect_ratio(
        camera_config["sensor_aspect_ratio"].as<std::string>())
    : 16. / 9.;

  this->set_sensor_size(
      camera_config["sensor_size"] ?
        camera_config["sensor_size"].as<double>() : 0.00635,
      sensor_aspect_ratio);

  cv::Mat camera_matrix = camera_config["camera_matrix"].as<cv::Mat>();
  this->pixel_width = this->sensor_width /
    camera_config["resolution"]["width"].as<double>();
  this->pixel_height = this->sensor_height /
    camera_config["resolution"]["height"].as<double>();

  this->focus_width = this->pixel_width * camera_matrix.at<double>(0, 0);
  this->focus_height = this->pixel_height * camera_matrix.at<double>(1, 1);
}

void GazePointCalculation::set_sensor_size(
    double sensor_diagonal,
    double aspect_ratio) {
  this->sensor_height = std::sqrt(sensor_diagonal * sensor_diagonal
      / (1 + aspect_ratio * aspect_ratio));
  this->sensor_width = aspect_ratio * this->sensor_height;
}

void GazePointCalculation::process(util::Data& data) {
  if (data.landmarks.num_parts() < 5) {
    return;
  }

  double interex_dist_m = 0.08671;  // Data taken from FaceBase
  double interex_dist_px = dlib::length(data.landmarks.part(36) -
      data.landmarks.part(45));
  double distance = (this->focus_width * interex_dist_m) / (interex_dist_px *
      this->pixel_width);
}

void GazePointCalculation::visualize(util::Data& data) {
  this->widget->clear_overlay();
  if (data.landmarks.num_parts() < 5) {
    return;
  }

  // TODO(shoeffner): This is just a WIP copy!
  double interex_dist_m = 0.08671;  // Data taken from FaceBase
  double interex_dist_px = dlib::length(data.landmarks.part(36) -
      data.landmarks.part(45));
  double distance = (this->focus_width * interex_dist_m) / (interex_dist_px *
      this->pixel_width);

  cv::Matx33d K = util::get_config()["meta"]["camera"]["camera_matrix"].as<cv::Mat>();  // NOLINT
  cv::Matx33d R;
  cv::Rodrigues(data.head_rotation, R);
  cv::Mat D =
    util::get_config()["meta"]["camera"]["distortion_coefficients"].as<cv::Mat>();  // NOLINT

  // Convert landmarks to OpenCV
  std::vector<cv::Point2d> landmarks;
  for (auto i = decltype(data.landmarks.num_parts()){0};
      i < data.landmarks.num_parts(); ++i) {
    cv::Point2d pv(data.landmarks.part(i).x(), data.landmarks.part(i).y());
    landmarks.push_back(pv);
  }
  // Cancel distortions and convert pixels to meters
  cv::undistortPoints(landmarks, landmarks, K, D);

  cv::Matx33d Rinv(R.inv());
  cv::Vec3d T = data.head_translation;
  std::vector<cv::Vec3d> restored_landmarks;
  for (auto p : landmarks) {
    cv::Vec3d v(p.x, p.y, 0);
    v = (Rinv * (v - T)) * distance;
    restored_landmarks.push_back(v);
  }

  std::vector<dlib::perspective_display::overlay_dot> overlay_dots;
  cv::Vec3d ref = restored_landmarks[30];
  for (auto p : restored_landmarks) {
    dlib::perspective_display::overlay_dot dot(
        {p[0] - ref[0], p[1] - ref[1], p[2] - ref[2]},
        dlib::rgb_pixel(0, 0, 255));
    overlay_dots.push_back(dot);
  }

  std::vector<cv::Vec3d> model =
    util::get_config()["pipeline"][2]["model"].as<std::vector<cv::Vec3d>>();
  for (auto m : model) {
    overlay_dots.push_back({{m[0], m[1], m[2]}, dlib::rgb_pixel(0, 255, 0)});
  }

  cv::Vec3d offset(0.02819, 0, 0);
  std::vector<cv::Vec3d> centers;
  centers.push_back(util::eye_ball_center(model[3], model[3] - offset));
  centers.push_back(util::eye_ball_center(model[2], model[2] + offset));
  for (auto m : centers) {
    overlay_dots.push_back({{m[0], m[1], m[2]}, dlib::rgb_pixel(255, 0, 0)});
  }

  this->widget->add_overlay(overlay_dots);
}

}  // namespace pipeline

}  // namespace gaze
