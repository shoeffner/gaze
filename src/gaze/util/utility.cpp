// Copyright 2017 Sebastian Höffner

#include "gaze/util/utility.h"

#include <algorithm>
#include <vector>

#include "dlib/image_processing.h"
#include "dlib/geometry.h"
#include "dlib/opencv.h"
#include "opencv2/core.hpp"


namespace gaze {

namespace util {

const dlib::rectangle convert(const cv::Rect in) {
  return dlib::rectangle(in.x,
                         in.y + in.height,
                         in.x + in.width,
                         in.y);
}

const cv::Rect convert(const dlib::rectangle in) {
  return cv::Rect(in.left(),
                  in.top(),
                  in.right() - in.left(),
                  in.bottom() - in.top());
}

const cv::Point convert(const dlib::point in) {
  return cv::Point(in.x(), in.y());
}

const dlib::point convert(const cv::Point in) {
  return dlib::point(in.x, in.y);
}

std::vector<dlib::chip_details> get_eyes_chip_details(
    const dlib::full_object_detection object_detection) {
  std::vector<dlib::chip_details> details;
  if (object_detection.num_parts() == 5) {
    auto get_rectangle = [](dlib::point one, dlib::point two)
      -> dlib::rectangle {
        dlib::rectangle result(one, two);
        double scale = (one - two).length() * 1.5;
        return dlib::centered_rect(result, scale, scale);
      };
    // left eye: 0 and 1
    details.push_back(dlib::chip_details(
          get_rectangle(object_detection.part(0),
                        object_detection.part(1))));

    // right eye: 3 and 2
    details.push_back(dlib::chip_details(
          get_rectangle(object_detection.part(2),
                        object_detection.part(3))));
  }
  return details;
}

const cv::Rect crop_to_image_boundary(const cv::Mat& image,
                                      const cv::Rect& roi) {
  return roi & cv::Rect(0, 0, image.cols, image.rows);
}

bool in_image(const cv::Mat& image, const cv::Point& poi) {
  return cv::Rect(0, 0, image.cols, image.rows).contains(poi);
}

}  // namespace util

}  // namespace gaze
