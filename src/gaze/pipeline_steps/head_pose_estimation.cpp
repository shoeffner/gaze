// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/head_pose_estimation.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "dlib/gui_widgets.h"
#include "dlib/image_processing.h"
#include "opencv2/opencv.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"


/**
 * @namespace YAML
 *
 * This namespace is defined by the yaml-cpp library. It is used to
 * specialize the converter template to make it easier to read
 * cv::Point3d data from a yaml file.
 */
namespace YAML {

/**
 * @struct convert
 *
 * Allows to convert between cv::Point3d and YAML::Node.
 */
template<>
struct convert<cv::Point3d> {
  static Node encode(const cv::Point3d& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
  }

  static bool decode(const Node& node, cv::Point3d& rhs) {  // NOLINT
    if (!node.IsSequence() || node.size() != 3) {
      return false;
    }
    rhs.x = node[0].as<double>();
    rhs.y = node[1].as<double>();
    rhs.z = node[2].as<double>();
    return true;
  }
};

}  // namespace YAML


namespace gaze {

namespace pipeline {

namespace {

cv::Matx33d camera_matrix(const util::Data& data) {
  cv::Matx33d camera_matrix;
  camera_matrix << data.source_image.cols, 0, data.source_image.cols / 2,
                   0, data.source_image.cols, data.source_image.rows / 2,
                   0, 0, 1;
  return camera_matrix;
}

cv::Mat distortions(const util::Data&) {
  return cv::Mat::zeros(4, 1, CV_32F);
}

}  // namespace


HeadPoseEstimation::HeadPoseEstimation() {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "HeadPoseEstimation";

  this->index_list = config["landmark_indices"].as<std::vector<int>>();

  if (config["model"]) {
    this->model_points = config["model"].as<std::vector<cv::Point3d>>();
    std::cout << config["model"] << std::endl;
  }

  if (config["model_scale"]) {
    this->model_scale = config["model_scale"].as<decltype(this->model_scale)>();
  }

  this->coord_base_overlay.push_back({{-1, -1}, {-1, -1},
                                      dlib::rgb_pixel(255, 0, 0)});
  this->coord_base_overlay.push_back({{-1, -1}, {-1, -1},
                                      dlib::rgb_pixel(0, 255, 0)});
  this->coord_base_overlay.push_back({{-1, -1}, {-1, -1},
                                      dlib::rgb_pixel(0, 0, 255)});
}

void HeadPoseEstimation::update_overlay(const util::Data& data) {
  this->widget->clear_overlay();
  if (data.landmarks.num_parts() <= 0) {
    return;
  }

  // TODO(shoeffner): store orientation/translation with the data object
  // Convert dlib::points to cv::Point for solvePNP
  std::vector<cv::Point2d> im_points;
  for (auto i : this->index_list) {
    im_points.push_back(cv::Point2d(data.landmarks.part(i).x(),
                                    data.landmarks.part(i).y()));
  }

  cv::Mat rotation;
  cv::Mat translation;
  cv::solvePnP(this->model_points, im_points,
      camera_matrix(data), distortions(data),
      rotation, translation, false, cv::SOLVEPNP_ITERATIVE);

  // Project reference points to image.
  std::vector<cv::Point3d> base_vecs =
    {{0, 0, 0},
     {0.1 * this->model_scale, 0, 0},
     {0, 0.1 * this->model_scale, 0},
     {0, 0, 0.1 * this->model_scale}};
  std::vector<cv::Point2d> image_points;
  cv::projectPoints(base_vecs, rotation, translation,
      camera_matrix(data), distortions(data), image_points);

  // coordinate system
  for (auto i = decltype(this->coord_base_overlay.size()){0};
       i < this->coord_base_overlay.size(); ++i) {
    this->coord_base_overlay[i] = dlib::image_display::overlay_line(
        dlib::point(image_points[0].x, image_points[0].y),
        dlib::point(image_points[i + 1].x, image_points[i + 1].y),
        this->coord_base_overlay[i].color);
  }
  this->widget->add_overlay(this->coord_base_overlay);

  // detected vs. projected points in overlay
  std::vector<cv::Point2d> projected_points;
  cv::projectPoints(this->model_points, rotation, translation,
      camera_matrix(data), distortions(data), projected_points);

  auto color_detection = dlib::rgb_pixel(255, 0, 0);
  auto color_projection = dlib::rgb_pixel(0, 0, 255);
  std::vector<dlib::image_display::overlay_circle> detections;
  std::vector<dlib::image_display::overlay_circle> projections;
  for (auto i = decltype(projected_points.size()){0};
       i < projected_points.size(); ++i) {
    detections.push_back(dlib::image_display::overlay_circle(
            data.landmarks.part(this->index_list[i]), 2, color_detection,
            "d" + std::to_string(this->index_list[i])));
    projections.push_back(dlib::image_display::overlay_circle(
          dlib::point(projected_points[i].x, projected_points[i].y),
          2, color_projection, "p" + std::to_string(this->index_list[i])));
  }
  this->widget->add_overlay(detections);
  this->widget->add_overlay(projections);
}

void HeadPoseEstimation::process(util::Data& data) {
  if (data.landmarks.num_parts() <= 0) {
    return;
  }
  return;
  std::vector<cv::Point2d> image_points;
  for (auto i = decltype(data.landmarks.num_parts()){0};
       i < data.landmarks.num_parts(); ++i) {
    image_points.push_back(cv::Point2d(data.landmarks.part(i).x(),
                                       data.landmarks.part(i).y()));
  }

  cv::Mat rotation;
  cv::Mat translation;
  cv::solvePnP(this->model_points, image_points,
      camera_matrix(data), distortions(data),
      rotation, translation);
}

void HeadPoseEstimation::visualize(util::Data& data) {
  this->widget->set_image(data.image);
  this->update_overlay(data);
}

}  // namespace pipeline

}  // namespace gaze
