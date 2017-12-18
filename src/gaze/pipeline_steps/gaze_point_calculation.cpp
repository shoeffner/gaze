#include "gaze/pipeline_steps/gaze_point_calculation.h"

#include <cmath>
#include <string>

#include "dlib/gui_widgets.h"
#include "opencv2/opencv.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"
#include "gaze/util/dlibcv.h"
#include "gaze/util/pipeline_utils.h"


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

}  // namespace util

namespace pipeline {

GazePointCalculation::GazePointCalculation()
    : eye_ball_centers(2) {
  YAML::Node config = util::get_config(this->number);
  YAML::Node meta_config = util::get_config()["meta"];

  // Step configuration
  this->name = config["name"] ?
    config["name"].as<std::string>() : "GazePoint";

  if (config["eye_ball_centers"]) {
    this->eye_ball_centers =
      config["eye_ball_centers"].as<std::vector<cv::Vec3d>>();
  } else {
    this->eye_ball_centers[0] = cv::Vec3d(-0.029205, 0.0327, -0.0395);
    this->eye_ball_centers[1] = cv::Vec3d(0.029205, 0.0327, -0.0395);
  }

  if (config["model"]) {
    this->model = config["model"].as<std::vector<cv::Vec3d>>();
    this->landmark_indices = config["landmark_indices"].as<std::vector<decltype(util::Data::landmarks.num_parts())>>();  // NOLINT
  } else {
    this->model = {
      {0.0, 0.0, 0.0},
      {0.0, -0.0636, -0.0125},
      {-0.0433, 0.0327, -0.026},
      {0.0433, 0.0327, -0.026},
      {-0.0289, -0.0289, -0.0241},
      {0.0289, -0.0289, -0.0241}
    };
  }

  // Camera configuration
  YAML::Node camera_config = meta_config["camera"];
  double sensor_aspect_ratio = camera_config["sensor_aspect_ratio"] ?
    util::parse_aspect_ratio(
        camera_config["sensor_aspect_ratio"].as<std::string>())
    : 16. / 9.;

  this->set_sensor_size(
      camera_config["sensor_size"] ?
        camera_config["sensor_size"].as<double>() : 0.00635,
      sensor_aspect_ratio);

  this->camera_matrix = camera_config["camera_matrix"].as<cv::Mat>();
  this->distortion_coefficients =
    camera_config["distortion_coefficients"].as<cv::Mat>();

  this->pixel_width = this->sensor_width /
    camera_config["resolution"]["width"].as<double>();
  this->pixel_height = this->sensor_height /
    camera_config["resolution"]["height"].as<double>();

  this->focus_width = this->pixel_width * this->camera_matrix(0, 0);
  this->focus_height = this->pixel_height * camera_matrix(1, 1);

  this->camera_offset_x = camera_config["position"]["x"].as<double>();
  this->camera_offset_y = camera_config["position"]["y"].as<double>();

  // Screen configuration
  YAML::Node screen_config = meta_config["screen"];
  this->screen_width_m =
    screen_config["measurements"]["width"].as<double>();
  this->screen_height_m =
    screen_config["measurements"]["height"].as<double>();
}

double GazePointCalculation::calculate_distance(
    const dlib::point& p0, const dlib::point& p1,
    double expected_model_distance) {
  double interex_dist_px = dlib::length(p0 - p1);
  double distance = (this->focus_width * expected_model_distance) /
    (interex_dist_px * this->pixel_width);
  return distance;
}

cv::Matx34d GazePointCalculation::invertProjection(
    const std::vector<cv::Vec3d>& landmarks) {
  cv::Matx34d transformation;
  cv::Mat inliers;
  cv::estimateAffine3D(landmarks, this->model, transformation, inliers);
  return transformation;
}

std::vector<cv::Vec3d> GazePointCalculation::unprojectPoints(
    const std::vector<cv::Vec2d>& points, const cv::Vec3d& translation,
    const cv::Matx33d& rotation, double distance) {
  std::vector<cv::Vec2d> restored_points;
  cv::undistortPoints(points, restored_points,
      this->camera_matrix, this->distortion_coefficients);
  std::vector<cv::Vec3d> unprojectedPoints;
  for (const cv::Vec2d& point : restored_points) {
    // Using -1 to adjust for mirror image
    cv::Vec3d unprojected(point[0], point[1], -1);
    unprojectedPoints.push_back(
        rotation * ((unprojected / distance) - translation));
  }
  return unprojectedPoints;
}

cv::Vec3d GazePointCalculation::get_model_to_camera_dir(
    const util::Data& data, const cv::Vec3d& translation,
    const cv::Matx33d& rotation, double distance) {
  std::vector<cv::Vec2d> ref = {
    cv::Vec2d(0, 0),
    cv::Vec2d(data.source_image.cols, 0),
    cv::Vec2d(0, data.source_image.rows)};

  std::vector<cv::Vec3d> ref3D =
    this->unprojectPoints(ref, translation, rotation, distance);

  // (2 - 0) x (1 - 0) to adjust for mirror image
  return cv::normalize((ref3D[2] - ref3D[0]).cross(ref3D[1] - ref3D[0]));
}

cv::Vec3d GazePointCalculation::get_camera_pos(
    const cv::Vec3d& model_to_camera_dir, double distance) {
  return distance * model_to_camera_dir;
}

std::vector<cv::Vec3d> GazePointCalculation::get_screen_corners(
    const cv::Vec3d& camera_pos, const cv::Vec3d& translation,
    const cv::Matx33d& rotation, double distance) {
  std::vector<cv::Vec3d> screen_directions = this->unprojectPoints(
      {{0, 0}, {1, 0}, {0, 1}}, translation, rotation, distance);
  for (auto i = decltype(screen_directions.size()){1};
       i < screen_directions.size(); ++i) {
    screen_directions[i] =
      cv::normalize(screen_directions[i] - screen_directions[0]);
  }

  cv::Vec3d screen_tl = camera_pos
    - screen_directions[1] * this->camera_offset_x
    - screen_directions[2] * this->camera_offset_y;
  cv::Vec3d screen_tr = screen_tl
    + screen_directions[1] * screen_width_m;
  cv::Vec3d screen_br = screen_tr
    + screen_directions[2] * screen_height_m;
  cv::Vec3d screen_bl = screen_tl
    + screen_directions[2] * screen_height_m;
  return {screen_tl, screen_tr, screen_br, screen_bl};
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

  // Convert landmarks to OpenCV data structures
  std::vector<cv::Vec3d> landmarks;
  for (auto i : this->landmark_indices) {
    cv::Vec3d landmark(data.landmarks.part(i).x(), data.landmarks.part(i).y(),
        0);
    landmarks.push_back(landmark);
  }

  // Project pupils into model
  cv::Matx34d transformation = this->invertProjection(landmarks);

  std::vector<dlib::chip_details> eyes =
    util::get_eyes_chip_details(data.landmarks);

  for (auto i = decltype(data.pupils.size()){0};
       i < data.pupils.size(); ++i) {
    dlib::point eye = data.centers[i] + eyes[i].rect.tl_corner();
    data.pupils[i] = transformation * cv::Vec4d(eye.x(), eye.y(), 0, 1);
  }

  // Calculate distance
  double distance = this->calculate_distance(
      data.landmarks.part(36), data.landmarks.part(45), 0.08671);
  cv::Matx33d R;
  cv::Rodrigues(data.head_rotation, R);

  cv::Vec3d model_to_camera_dir = this->get_model_to_camera_dir(
      data, data.head_translation, R, distance);
  cv::Vec3d camera_pos = this->get_camera_pos(model_to_camera_dir, distance);

  std::vector<cv::Vec3d> screen_tl_tr_br_bl =
    this->get_screen_corners(camera_pos, data.head_translation, R, distance);
}

void GazePointCalculation::visualize(util::Data& data) {
  // TODO(shoeffner): Add pipeline step to visualize 2D gaze point / area
  this->widget->clear_overlay();
  if (data.landmarks.num_parts() < 5) {
    return;
  }

  // Recalculate distance, etc.
  double distance = this->calculate_distance(
      data.landmarks.part(36), data.landmarks.part(45), 0.08671);
  cv::Matx33d R;
  cv::Rodrigues(data.head_rotation, R);

  // Move landmarks into model coordinates for an intuitive visualization of
  // where the direction of the camera plane is
  std::vector<cv::Vec2d> image_landmarks;
  for (auto i = decltype(data.landmarks.num_parts()){0};
       i < data.landmarks.num_parts(); ++i) {
    image_landmarks.push_back(
        cv::Vec2d(data.landmarks.part(i).x(), data.landmarks.part(i).y()));
  }
  std::vector<cv::Vec3d> world_landmarks = this->unprojectPoints(
      image_landmarks, data.head_translation, R, distance);

  // Find camera direction
  cv::Vec3d camera_dir = this->get_model_to_camera_dir(
      data, data.head_translation, R, distance);

  cv::Vec3d camera_pos = this->get_camera_pos(camera_dir, distance);

  // Calculate screen corners
  std::vector<cv::Vec3d> screen_corners =
    this->get_screen_corners(camera_pos, data.head_translation, R, distance);

  // Helpers to draw points
  std::vector<dlib::perspective_display::overlay_dot>
    overlay_dots;

  auto add_to_overlay =
    [&overlay_dots] (std::vector<cv::Vec3d> points,
                     dlib::rgb_pixel color) -> void {
      for (auto p : points) {
        overlay_dots.push_back({{p[0], p[1], p[2]}, color});
      }
    };

  add_to_overlay(this->model, {255, 255, 0});
  add_to_overlay(data.pupils, {0, 255, 255});
  add_to_overlay(this->eye_ball_centers, {255, 0, 255});
  add_to_overlay(world_landmarks, {0, 255, 0});

  // Visualize camera position
  cv::Vec3d camera = this->get_camera_pos(camera_dir, distance);
  overlay_dots.push_back({{camera[0], camera[1], camera[2]},
      dlib::rgb_pixel(255, 255, 255)});

  this->widget->add_overlay(overlay_dots);

  auto to_line = [](const cv::Vec3d& p0, const cv::Vec3d& p1,
      dlib::rgb_pixel color) ->
    dlib::perspective_display::overlay_line {
      return dlib::perspective_display::overlay_line(
          {p0[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, color);
    };

  // Draw coordinate axes
  cv::Vec3d origin(0, 0, 0);
  cv::Vec3d xdir(.1, 0, 0);
  cv::Vec3d ydir(0, .1, 0);
  cv::Vec3d zdir(0, 0, .1);

  this->widget->add_overlay({
      to_line(origin, xdir, {255, 0, 0}),
      to_line(origin, ydir, {0, 255, 0}),
      to_line(origin, zdir, {0, 0, 255}),
      to_line(origin, origin + camera_dir, {255, 255, 0})});

  this->widget->add_overlay({
      to_line(screen_corners[0], screen_corners[1], {255, 255, 0}),
      to_line(screen_corners[1], screen_corners[2], {255, 255, 0}),
      to_line(screen_corners[2], screen_corners[3], {255, 255, 0}),
      to_line(screen_corners[3], screen_corners[0], {255, 255, 0})});
}

}  // namespace pipeline

}  // namespace gaze
