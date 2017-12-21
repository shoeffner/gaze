#include <cmath>
#include <functional>
#include <memory>
#include <utility>

#include "gaze/gaze.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"


namespace {
  cv::Mat Blank(int height, int width) {
     return cv::Mat(height, width, CV_8UC3, cv::Scalar::all(0));
  }

  cv::Mat Spiral(int height, int width) {
    cv::Mat image = cv::Mat::zeros(height, width, CV_8UC3);

    double a = 1;
    double b = 0.3;
    double theta_step = 0.05;
    const int num_points = 500;

    auto x = [a, b](double theta) -> double {
      return a * std::cos(theta) * std::exp(b * theta);
    };
    auto y = [a, b](double theta) -> double {
      return a * std::sin(theta) * std::exp(b * theta);
    };

    cv::Point points[num_points] = {};
    double theta = 0;
    double half_width = width / 2.0;
    double half_height = height / 2.0;
    for (int i = 0; i < num_points; ++i) {
      points[i] = cv::Point(half_width + x(theta), half_height + y(theta));
      theta += theta_step;
    }
    const cv::Point* ppt[1] = {points};
    const int num_points_ptr[1] = {num_points};
    cv::polylines(image, ppt, num_points_ptr, 1,
                  false, cv::Scalar(255, 255, 0));
    return image;
  }

  cv::Point map_and_convert(std::pair<int, int> point,
      int to_width, int to_height,
      int from_width, int from_height) {
    auto map = [](int value, int from, int to) -> int {
      return value * to / from;
    };
    return cv::Point(map(point.first, from_width, to_width),
                     map(point.second, from_height, to_height));
  }

  void main_loop(const std::function<cv::Mat(int, int)> drawing,
                 int width = 400,
                 int height = 400,
                 int screen_width = 1920,
                 int screen_height = 1080) {
    cv::namedWindow("Simple Tracker");

    std::unique_ptr<gaze::GazeTracker> tracker(
        new gaze::GazeTracker("st_subject", true));

    cv::Mat image = drawing(height, width);
    cv::Mat storage = image.clone();

    tracker->start_trial("simple tracker");
    while (true) {
      storage.copyTo(image);

      std::pair<int, int> gaze_pair = tracker->get_current_gaze_point();
      cv::Point gaze_point = map_and_convert(gaze_pair,
          width, height, screen_width, screen_height);
      cv::drawMarker(image, gaze_point, cv::Scalar(0, 255, 0));

      cv::imshow("Simple Tracker", image);

      if (cv::waitKey(15) != -1) {
        break;
      }
    }
    tracker->stop_trial();
    cv::destroyWindow("Simple Tracker");
  }
}  // namespace

int main(const int, const char** const) {
  int width = 960;
  int height = 540;
  int screen_width = 2880;
  int screen_height = 1800;
  main_loop(&::Blank, width, height, screen_width, screen_height);
  return 0;
}
