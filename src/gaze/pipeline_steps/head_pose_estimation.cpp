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


namespace gaze {

namespace pipeline {

HeadPoseEstimation::HeadPoseEstimation() {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "HeadPoseEstimation";

  if (config["landmark_indices"]) {
    this->index_list = config["landmark_indices"].as<std::vector<int>>();
  } else {
    this->index_list = {30, 8, 36, 45, 48, 54};
  }

  if (config["model"]) {
    this->model_points = config["model"].as<std::vector<cv::Point3d>>();
  } else {
    this->model_points = {
      {0.0, 0.0, 0.0},
      {0.0, -0.0636, -0.0125},
      {-0.0433, 0.0327, -0.026},
      {0.0433, 0.0327, -0.026},
      {-0.0289, -0.0289, -0.0241},
      {0.0289, -0.0289, -0.0241}
    };
  }

  if (config["model_scale"]) {
    this->model_scale = config["model_scale"].as<decltype(this->model_scale)>();
  } else {
    this->model_scale = 1;
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

  // Project reference points to image.
  std::vector<cv::Point3d> base_vecs =
    {{0, 0, 0},
     {0.1 * this->model_scale, 0, 0},
     {0, 0.1 * this->model_scale, 0},
     {0, 0, 0.1 * this->model_scale}};
  std::vector<cv::Point2d> image_points;
  cv::projectPoints(base_vecs, data.head_rotation, data.head_translation,
      this->read_or_set_camera_matrix(data),
      this->get_and_maybe_read_distortions(data), image_points);

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
  cv::projectPoints(this->model_points, data.head_rotation,
      data.head_translation, this->read_or_set_camera_matrix(data),
      this->get_and_maybe_read_distortions(data), projected_points);

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

  // Convert dlib::points to cv::Point for solvePNP
  std::vector<cv::Point2d> im_points;
  for (auto i : this->index_list) {
    im_points.push_back(cv::Point2d(data.landmarks.part(i).x(),
                                    data.landmarks.part(i).y()));
  }

  cv::solvePnP(this->model_points, im_points,
      this->read_or_set_camera_matrix(data),
      this->get_and_maybe_read_distortions(data),
      data.head_rotation, data.head_translation, false, cv::SOLVEPNP_ITERATIVE);
}

void HeadPoseEstimation::visualize(util::Data& data) {
  this->widget->set_image(data.image);
  this->update_overlay(data);
}

cv::Matx33d HeadPoseEstimation::read_or_set_camera_matrix(
    const util::Data& data) {
  if (this->has_camera_matrix) {
    return this->camera_matrix;
  }
  YAML::Node camera_config = util::get_config()["meta"]["camera"];
  if (camera_config["camera_matrix"]) {
    this->camera_matrix = camera_config["camera_matrix"].as<cv::Mat>();
  } else {
    this->camera_matrix <<
      data.source_image.cols, 0, data.source_image.cols / 2,
      0, data.source_image.cols, data.source_image.rows / 2,
      0, 0, 1;
  }
  this->has_camera_matrix = true;
  return this->camera_matrix;
}

cv::Mat HeadPoseEstimation::get_and_maybe_read_distortions(const util::Data&) {
  if (this->has_distortions) {
    return this->distortions;
  }
  YAML::Node camera_config = util::get_config()["meta"]["camera"];
  if (camera_config["distortion_coefficients"]) {
    this->distortions = camera_config["distortion_coefficients"].as<cv::Mat>();
  } else {
    this->distortions = cv::Mat::zeros(4, 1, CV_64F);
  }
  this->has_distortions = true;
  return this->distortions;
}

}  // namespace pipeline

}  // namespace gaze
