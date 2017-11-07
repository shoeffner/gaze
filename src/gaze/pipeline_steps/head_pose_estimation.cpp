// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/head_pose_estimation.h"

#include <vector>

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

}  // namespace


HeadPoseEstimation::HeadPoseEstimation()
    : nose_tip_y_offset(-10) {
  this->name = "Head pose";

  this->overlay.push_back({{-1, -1}, {-1, -1}, dlib::rgb_pixel(255, 0, 0)});
  this->overlay.push_back({{-1, -1}, {-1, -1}, dlib::rgb_pixel(0, 255, 0)});
  this->overlay.push_back({{-1, -1}, {-1, -1}, dlib::rgb_pixel(0, 0, 255)});

  // TODO(shoeffner): Find way to remove fake nosetip
  this->model_points.push_back({0, 3.8, -10.7});  // nose tip (faked in landmarks)

  // TODO(shoeffner): Find suitable model values
  this->model_points.push_back({5.95, -3.57, -5.55});  // left outer corner
  this->model_points.push_back({1.3, -3.57, -6.05});  // left inner corner
  this->model_points.push_back({-5.95, -3.57, -5.55});  // right outer corner
  this->model_points.push_back({-1.3, -3.57, -6.05});  // right inner corner
  this->model_points.push_back({0, 4.35, -8.7});  // septum base
}

void HeadPoseEstimation::update_overlay(const util::Data& data) {
  this->widget->clear_overlay();
  if (data.landmarks.num_parts() <= 0) {
    return;
  }

  // TODO(shoeffner): store orientation/translation with the data object
  std::vector<cv::Point2d> im_points;
  im_points.push_back(
      cv::Point2d(data.landmarks.part(4).x(),
                  data.landmarks.part(4).y() + this->nose_tip_y_offset));
  for (auto i = decltype(data.landmarks.num_parts()){0};
       i < data.landmarks.num_parts(); ++i) {
    im_points.push_back(cv::Point2d(data.landmarks.part(i).x(),
                                    data.landmarks.part(i).y()));
  }
  cv::Mat rotation;
  cv::Mat translation;
  cv::solvePnP(this->model_points, im_points,
      camera_matrix(data), distortions(data),
      rotation, translation);

  // Project reference points to image.
  std::vector<cv::Point3f> ref_points =
    {{0, 0, 0}, {10, 0, 0}, {0, 10, 0}, {0, 0, 10}};
  std::vector<cv::Point2f> image_points;
  cv::projectPoints(ref_points, rotation, translation,
      camera_matrix(data), distortions(data), image_points);

  // Convert result to dlib overlay
  for (auto i = decltype(this->overlay.size()){0};
       i < this->overlay.size(); ++i) {
    this->overlay[i] = dlib::image_display::overlay_line(
        dlib::point(image_points[0].x, image_points[0].y),
        dlib::point(image_points[i + 1].x, image_points[i + 1].y),
        this->overlay[i].color);
  }
  this->widget->add_overlay(this->overlay);

  std::vector<dlib::image_display::overlay_circle> detections;
  detections.push_back(
      dlib::image_display::overlay_circle(
        {data.landmarks.part(4).x(),
         data.landmarks.part(4).y() + this->nose_tip_y_offset},
         2, dlib::rgb_alpha_pixel(255, 0, 0, 127)));
  for (auto i = decltype(data.landmarks.num_parts()){0};
       i < data.landmarks.num_parts(); ++i) {
    detections.push_back(
        dlib::image_display::overlay_circle(data.landmarks.part(i), 2,
          dlib::rgb_alpha_pixel(255, 0, 0, 127)));
  }
  this->widget->add_overlay(detections);

  std::vector<dlib::image_display::overlay_circle> projections;
  std::vector<cv::Point2f> projected_points;
  cv::projectPoints(this->model_points, rotation, translation,
      camera_matrix(data), distortions(data), projected_points);
  for (auto i = decltype(projected_points.size()){0};
       i < projected_points.size(); ++i) {
    projections.push_back(dlib::image_display::overlay_circle(
          {(long) projected_points[i].x, (long) projected_points[i].y}, 2,
          dlib::rgb_alpha_pixel(0, 255, 0, 127)));
  }
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
