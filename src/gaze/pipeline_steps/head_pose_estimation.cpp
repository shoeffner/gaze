// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/head_pose_estimation.h"

#include <vector>
#include <cstdlib>

#include "dlib/gui_widgets.h"
#include "dlib/image_processing.h"
#include "opencv2/opencv.hpp"

#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

namespace {

cv::Matx33f camera_matrix(const util::Data& data) {
  cv::Matx33f camera_matrix;
  camera_matrix << data.source_image.cols, 0, data.source_image.cols / 2,
                   0, data.source_image.cols, data.source_image.rows / 2,
                   0, 0, 1;
  return camera_matrix;
}

cv::Mat distortions(const util::Data&) {
  return cv::Mat::zeros(4, 1, CV_32F);
}

dlib::point estimate_nosetip(const util::Data& data) {
  auto x_offset = ((data.landmarks.part(2) - data.landmarks.part(3)).length() -
    (data.landmarks.part(0) - data.landmarks.part(1)).length()) / 4;
  auto y_offset = -
    std::abs(
        (data.landmarks.part(1).y() + data.landmarks.part(3).y()) / 2
         - data.landmarks.part(4).y()) * 2 / 5;

  return dlib::point(data.landmarks.part(4).x() + x_offset,
      data.landmarks.part(4).y() + y_offset);
}

}  // namespace


HeadPoseEstimation::HeadPoseEstimation() {
  this->name = "Head pose";

  this->overlay.push_back({{-1, -1}, {-1, -1}, dlib::rgb_pixel(255, 0, 0)});
  this->overlay.push_back({{-1, -1}, {-1, -1}, dlib::rgb_pixel(0, 255, 0)});
  this->overlay.push_back({{-1, -1}, {-1, -1}, dlib::rgb_pixel(0, 0, 255)});

  // TODO(shoeffner): Find suitable model values
  this->model_points.push_back({0.0000, 0.0000, 0.0000});  // exl
  this->model_points.push_back({-0.0465, 0.0000, -0.0050});  // enl
  this->model_points.push_back({-0.1190, 0.0000, 0.0000});  // exr
  this->model_points.push_back({-0.0725, 0.0000, -0.0050});  // enr
  this->model_points.push_back({-0.0595, 0.0792, -0.0315});  // sn
}

void HeadPoseEstimation::update_overlay(const util::Data& data) {
  this->widget->clear_overlay();
  if (data.landmarks.num_parts() <= 0) {
    return;
  }

  // TODO(shoeffner): store orientation/translation with the data object
  // Convert dlib::points to cv::Point for solvePNP
  std::vector<cv::Point2f> im_points;
  for (auto i = decltype(data.landmarks.num_parts()){0};
       i < data.landmarks.num_parts(); ++i) {
    im_points.push_back(cv::Point2f(data.landmarks.part(i).x(),
                                    data.landmarks.part(i).y()));
  }

  cv::Mat rotation;
  cv::Mat translation;
  cv::solvePnP(this->model_points, im_points,
      camera_matrix(data), distortions(data),
      rotation, translation, false, cv::SOLVEPNP_ITERATIVE);

  // Project reference points to image.
  std::vector<cv::Point3f> ref_points =
    {{0, 0, 0}, {10, 0, 0}, {0, 10, 0}, {0, 0, 10}};
  std::vector<cv::Point2f> image_points;
  cv::projectPoints(ref_points, rotation, translation,
      camera_matrix(data), distortions(data), image_points);

  // coordinate system
  for (auto i = decltype(this->overlay.size()){0};
       i < this->overlay.size(); ++i) {
    this->overlay[i] = dlib::image_display::overlay_line(
        dlib::point(image_points[0].x, image_points[0].y),
        dlib::point(image_points[i + 1].x, image_points[i + 1].y),
        this->overlay[i].color);
  }
  this->widget->add_overlay(this->overlay);

  // detected vs. projected points in overlay
  std::vector<cv::Point2f> projected_points;
  cv::projectPoints(this->model_points, rotation, translation,
      camera_matrix(data), distortions(data), projected_points);

  auto color_detection = dlib::rgb_pixel(255, 0, 0);
  auto color_projection = dlib::rgb_pixel(0, 0, 255);
  std::vector<dlib::image_display::overlay_circle> detections;
  std::vector<dlib::image_display::overlay_circle> projections;
  for (auto i = decltype(projected_points.size()){0};
       i < projected_points.size(); ++i) {
    detections.push_back(dlib::image_display::overlay_circle(
            data.landmarks.part(i), 2, color_detection, "d" +
            std::to_string(i)));
    projections.push_back(dlib::image_display::overlay_circle(
          {static_cast<long>(projected_points[i].x),
           static_cast<long>(projected_points[i].y)},
          2, color_projection, "p" + std::to_string(i)));
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
