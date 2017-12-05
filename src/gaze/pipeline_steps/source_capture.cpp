#include "gaze/pipeline_steps/source_capture.h"

#include <memory>
#include <string>

#include "dlib/opencv.h"
#include "opencv2/opencv.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

SourceCapture::SourceCapture() {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "SourceCapture";

  std::string source = config["source"] ?
    config["source"].as<std::string>() : "0";

  bool is_webcam = true;
  try {
    int cam_source = std::stoi(source);
    this->video_capture = cv::VideoCapture(cam_source);
  } catch (const std::invalid_argument&) {
    is_webcam = false;
    this->video_capture = cv::VideoCapture(source);
  }

  if (is_webcam) {
    YAML::Node camera_config = util::get_config()["meta"]["camera"];
    this->video_capture.set(cv::CAP_PROP_FPS,
        camera_config["fps"] ? camera_config["fps"].as<double>() : 60.0);
    this->video_capture.set(cv::CAP_PROP_FRAME_WIDTH,
        camera_config["resolution"]["width"] ?
        camera_config["resolution"]["width"].as<double>() : 640.0);
    this->video_capture.set(cv::CAP_PROP_FRAME_HEIGHT,
        camera_config["resolution"]["height"] ?
        camera_config["resolution"]["height"].as<double>() : 360.0);
  }
}

SourceCapture::~SourceCapture() {
  this->video_capture.release();
}

void SourceCapture::process(util::Data& data) {
  if (static_cast<int>(this->video_capture.get(cv::CAP_PROP_POS_AVI_RATIO))
      == 1) {
    return;
  }
  bool received = this->video_capture.read(data.source_image);
  if (received) {
    this->last_frame = data.source_image;
  } else {
    data.source_image = this->last_frame;
  }
  dlib::assign_image(
     data.image,
     dlib::cv_image<dlib::bgr_pixel>(data.source_image));
}

void SourceCapture::visualize(util::Data& data) {
  this->widget->set_image(data.image);
}

}  // namespace pipeline

}  // namespace gaze
