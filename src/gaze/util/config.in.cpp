// Copyright 2017 Sebastian Höffner

#include "gaze/util/config.h"

#include <iostream>

#include "yaml-cpp/yaml.h"


namespace gaze {

namespace util {

YAML::Node get_config() {
  YAML::Node config;
  try {
    config = YAML::LoadFile("gaze.yaml");
  } catch (YAML::BadFile) {
    std::cout << "loading default configuration." << std::endl;
    config = YAML::Load("@DEFAULT_CONFIGURATION_FILE@");
  }
  return config;
}

YAML::Node get_config(int pipeline_step_number) {
  return get_config()["pipeline"][pipeline_step_number];
}

}  // namespace util

}  // namespace gaze
