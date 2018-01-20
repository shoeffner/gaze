#include "gaze/pipeline_steps/gaze_capture.h"

#include <string>

#include "dlib/image_processing.h"
#include "dlib/image_transforms.h"
#include "dlib/opencv.h"
#include "opencv2/opencv.hpp"
#include "yaml-cpp/yaml.h"

#ifdef HAS_CAFFE
#include "itracker/itracker.h"
#endif

#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/config.h"
#include "gaze/util/data.h"
#include "gaze/util/pipeline_utils.h"


namespace gaze {

namespace pipeline {

GazeCapture::GazeCapture() {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "iTracker";

  // Read caffe model configurations
  std::string model_file = config["model"] ?
    config["model"].as<std::string>() : "models/itracker_deploy.prototxt";
  std::string weights_file = config["weights"] ?
    config["weights"].as<std::string>() :
    "models/snapshots/itracker25x_iter_92000.caffemodel";
  std::string mean_left_file = config["mean_left"] ?
    config["mean_left"].as<std::string>() :
    "models/mean_images/mean_left_224_new.binaryproto";
  std::string mean_right_file = config["mean_right"] ?
    config["mean_right"].as<std::string>() :
    "models/mean_images/mean_right_224.binaryproto";
  std::string mean_face_file = config["mean_face"] ?
    config["mean_face"].as<std::string>() :
    "models/mean_images/mean_face_224.binaryproto";

#ifdef HAS_CAFFE
  YAML::Node meta_config = util::get_config()["meta"];
  YAML::Node camera_config = meta_config["camera"];
  this->camera_offset = cv::Vec2d(camera_config["position"]["x"].as<double>(),
                                  camera_config["position"]["y"].as<double>());

  YAML::Node screen_config = meta_config["screen"];
  this->screen_size_m =
    cv::Vec2d(screen_config["measurements"]["width"].as<double>(),
              screen_config["measurements"]["height"].as<double>());
  this->target_size =
    cv::Vec2d(screen_config["resolution"]["width"].as<double>(),
              screen_config["resolution"]["height"].as<double>());
  // Init network
  this->itracker = std::unique_ptr<itracker::ITracker>(
      new itracker::ITracker(model_file, weights_file,
        mean_left_file, mean_right_file, mean_face_file));
#endif
}

void GazeCapture::process(util::Data& data) {
  if (data.landmarks.num_parts() < 5) {
    return;
  }

#ifdef HAS_CAFFE
  // Gather input
  std::vector<dlib::chip_details> chips =
    util::get_eyes_chip_details(data.landmarks);

  auto right_dlib = dlib::sub_image(data.image, chips[0].rect);
  auto left_dlib = dlib::sub_image(data.image, chips[1].rect);
  auto face_dlib = dlib::sub_image(data.image, data.landmarks.get_rect());
  cv::Mat right = dlib::toMat(right_dlib);
  cv::Mat left = dlib::toMat(left_dlib);
  cv::Mat face = dlib::toMat(face_dlib);

  dlib::matrix<unsigned char> mask_in =
    dlib::zeros_matrix<unsigned char>(data.image.nr(), data.image.nc());
  dlib::set_subm(mask_in, data.landmarks.get_rect()) = 1;
  dlib::matrix<unsigned char, 25, 25> mask_out;
  dlib::resize_image(mask_in, mask_out, dlib::interpolate_nearest_neighbor());
  cv::Mat mask = dlib::toMat(mask_out);

  cv::Vec2d prediction_cam = this->itracker->predict(left, right, face, mask);
  // Note that the prediction is in cm, thus DIV100 is needed
  double x = (prediction_cam[0] / 100 + this->camera_offset[0])
           / this->screen_size_m[0] * this->target_size[0];
  double y = (-prediction_cam[1] / 100 + this->camera_offset[1])
           / this->screen_size_m[1] * this->target_size[1];
  data.estimated_gaze_point = cv::Vec2d(x, y);
#endif
}

void GazeCapture::visualize(util::Data& data) {
#ifdef HAS_CAFFE
  std::cout << data.estimated_gaze_point << std::endl;
#endif
}

}  // namespace pipeline

}  // namespace gaze
