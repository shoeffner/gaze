#include "gaze/util/config.h"

#include <iostream>

#include "yaml-cpp/yaml.h"


namespace gaze {

namespace util {

YAML::Node get_config() {
  YAML::Node config;
  try {
    config = YAML::LoadFile("gaze_pipeline.yaml");
  } catch (YAML::BadFile) {
    std::cerr << "Can not load gaze_pipeline.yaml! "
                 "Loading gaze_pipeline.default.yaml" << std::endl;
    config = YAML::LoadFile("gaze_pipeline.default.yaml");
  }
  return config;
}

YAML::Node get_config(int pipeline_step_number) {
  return get_config()["pipeline"][pipeline_step_number];
}

}  // namespace util

}  // namespace gaze
