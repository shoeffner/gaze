// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_UTIL_CONFIG_H_
#define INCLUDE_GAZE_UTIL_CONFIG_H_

#include "yaml-cpp/yaml.h"


namespace gaze {

namespace util {

/**
 * Tries to read "gaze_pipeline.yaml" or on failure
 * "gaze_pipeline.default.yaml" and returns the
 * resulting YAML::Node.
 *
 * @returns the yaml configuration
 */
YAML::Node get_config();

/**
 * Calls get_config() and gets the data from
 * @code
 * pipeline:
 *   - ...
 *   - ...
 * @endcode
 * Where pipeline_step_number is the bullet to read.
 *
 * @param pipeline_step_number the pipeline step to read
 * @returns the sub node for the respective pipeline step.
 */
YAML::Node get_config(int pipeline_step_number);

}  // namespace util

}  // namespace gaze

#endif  // INCLUDE_GAZE_UTIL_CONFIG_H_
