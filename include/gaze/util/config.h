#ifndef INCLUDE_GAZE_UTIL_CONFIG_H_
#define INCLUDE_GAZE_UTIL_CONFIG_H_

#include "opencv2/opencv.hpp"
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


/**
 * @namespace YAML
 * @brief This namespace is defined by the yaml-cpp library.
 *
 * It is used to specialize the converter template.
 * // TODO(shoeffner): Add unit tests for YAML conversions.
 */
namespace YAML {

/**
 * @struct convert<cv::Point3d>
 * @brief Allows to convert between cv::Point3d and YAML::Node.
 */
template<>
struct convert<cv::Point3d> {
  /**
   * Encodes a cv::Point3d as a YAML node.
   *
   * @param rhs The point
   *
   * @returns A YAML node representing the point
   */
  static Node encode(const cv::Point3d& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
  }

  /**
   * Decodes a cv::Point3d from a YAML node.
   *
   * @param node The node to decode.
   * @param rhs The point to decode into.
   *
   * @returns true on success.
   */
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

/**
 * @struct convert<cv::Mat>
 * @brief Allows to convert between cv::Mat and YAML::Node.
 */
template<>
struct convert<cv::Mat> {
  /**
   * Encodes a cv::Mat as a YAML node.
   *
   * @param rhs The matrix
   * @returns A YAML node representing the matrix
   */
  static Node encode(const cv::Mat& rhs) {
    Node node;
    node["rows"] = rhs.rows;
    node["cols"] = rhs.cols;
    node["dt"] = 'd';

    Node data;
    for (auto d = rhs.datastart; d != rhs.dataend; ++d) {
      data.push_back(static_cast<double>(*d));
    }
    node["data"] = data;
    return node;
  }

  /**
   * Decodes a cv::Mat from a YAML node.
   *
   * @param node The node to decode.
   * @param rhs The matrix to decode into.
   *
   * @returns true on success.
   */
  static bool decode(const Node& node, cv::Mat& rhs) {  // NOLINT
    if (!node["rows"] || !node["cols"] || !node["dt"] || !node["data"] ||
        !node["data"].IsSequence()) {
      return false;
    }
    char dt = node["dt"].as<char>();
    if (dt != 'd') {
      return false;
    }
    int rows = node["rows"].as<int>();
    int cols = node["cols"].as<int>();
    if (static_cast<decltype(node["data"].size())>(rows * cols) !=
        node["data"].size()) {
      return false;
    }

    double data[rows * cols];
    for (auto i = decltype(node["data"].size()){0}; i < node["data"].size();
        ++i) {
      data[i] = node["data"][i].as<double>();
    }
    cv::Mat mat = cv::Mat(rows, cols, CV_64F, data);
    mat.assignTo(rhs);
    return true;
  }
};

}  // namespace YAML

#endif  // INCLUDE_GAZE_UTIL_CONFIG_H_
