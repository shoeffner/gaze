#include "gaze/pipeline_steps/gaze_point_calculation.h"

#include <string>

#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
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
  std::cout << this->focus_width << std::endl;
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
  if (data.landmarks.num_parts() < 5) {
    return;
  }
  // TODO(shoeffner): This is just a WIP copy!
  double interex_dist_m = 0.08671;  // Data taken from FaceBase
  double interex_dist_px = dlib::length(data.landmarks.part(36) -
      data.landmarks.part(45));
  double distance = (this->focus_width * interex_dist_m) / (interex_dist_px *
      this->pixel_width);
  this->widget->set_text("Distance: " + std::to_string(distance));
}

}  // namespace pipeline

}  // namespace gaze
