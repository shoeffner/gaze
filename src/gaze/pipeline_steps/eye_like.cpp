// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/eye_like.h"

#include <cmath>
#include "opencv2/opencv.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/pipeline_steps/pupil_localization.h"
#include "gaze/util/config.h"
#include "gaze/util/data.h"

namespace gaze {

namespace eyelike {

double compute_dynamic_threshold(const cv::Mat& mat, double std_dev_factor) {
  cv::Scalar stdMagnGrad, meanMagnGrad;
  cv::meanStdDev(mat, meanMagnGrad, stdMagnGrad);
  // TODO(shoeffner): double check
  // double std_dev = stdMagnGrad[0] / sqrt(mat.rows*mat.cols);
  double std_dev = stdMagnGrad[0];
  return std_dev_factor * std_dev + meanMagnGrad[0];
}

cv::Mat compute_x_gradient(const cv::Mat& image) {
  cv::Mat out(image.rows, image.cols, CV_64F);

  for (auto y = decltype(image.rows){0}; y < image.rows; ++y) {
    const uchar *row = image.ptr<uchar>(y);
    double *out_row = out.ptr<double>(y);

    out_row[0] = row[1] - row[0];
    for (auto x = decltype(image.cols){1}; x < image.cols - 1; ++x) {
      out_row[x] = (row[x + 1] - row[x - 1]) / 2.0;
    }
    out_row[image.cols - 1] = row[image.cols - 1] - row[image.cols - 2];
  }

  return out;
}

cv::Mat matrix_magnitude(const cv::Mat& matX, const cv::Mat& matY) {
  cv::Mat mags(matX.rows, matX.cols, CV_64F);
  for (auto y = decltype(matX.rows){0}; y < matX.rows; ++y) {
    const double *Xr = matX.ptr<double>(y), *Yr = matY.ptr<double>(y);
    double *row = mags.ptr<double>(y);
    for (auto x = decltype(matX.cols){0}; x < matX.cols; ++x) {
      double gX = Xr[x], gY = Yr[x];
      double magnitude = std::sqrt((gX * gX) + (gY * gY));
      row[x] = magnitude;
    }
  }
  return mags;
}

void test_possible_centers_formula(
    int x, int y,
    const cv::Mat& weight,
    double gx, double gy,
    cv::Mat& out) {  // NOLINT
  for (int cy = 0; cy < out.rows; ++cy) {
    double *out_row = out.ptr<double>(cy);
    const unsigned char *Wr = weight.ptr<unsigned char>(cy);
    for (int cx = 0; cx < out.cols; ++cx) {
      if (x == cx && y == cy) {
        continue;
      }
      double dx = x - cx;
      double dy = y - cy;
      // normalize d
      double magnitude = std::sqrt((dx * dx) + (dy * dy));
      dx = dx / magnitude;
      dy = dy / magnitude;
      double dotProduct = dx * gx + dy * gy;
      dotProduct = std::max(0.0, dotProduct);
      // square and multiply by the weight
      out_row[cx] += dotProduct * dotProduct * Wr[cx];
    }
  }
}

cv::Point unscale_point(cv::Point point, cv::Rect orig_size,
                        double fast_eye_width) {
  double ratio = fast_eye_width / orig_size.width;
  int x = round(point.x / ratio);
  int y = round(point.y / ratio);
  return cv::Point(x, y);
}

cv::Point find_eye_center(
    const cv::Mat& face, const cv::Rect& eye,
    double gradient_threshold, double fast_eye_width) {
  cv::Mat eyeROIUnscaled = face(eye);
  cv::Mat eyeROI;
  cv::resize(
      eyeROIUnscaled,
      eyeROI,
      cv::Size(fast_eye_width,
               fast_eye_width / eyeROIUnscaled.cols * eyeROIUnscaled.rows));


  cv::Mat gradientX = compute_x_gradient(eyeROI);
  cv::Mat gradientY = compute_x_gradient(eyeROI.t()).t();
  cv::Mat mags = matrix_magnitude(gradientX, gradientY);
  double gradientThresh = compute_dynamic_threshold(mags,
      gradient_threshold);

  for (int y = 0; y < eyeROI.rows; ++y) {
    double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
    const double *Mr = mags.ptr<double>(y);
    for (int x = 0; x < eyeROI.cols; ++x) {
      double gX = Xr[x], gY = Yr[x];
      double magnitude = Mr[x];
      if (magnitude > gradientThresh) {
        Xr[x] = gX / magnitude;
        Yr[x] = gY / magnitude;
      } else {
        Xr[x] = 0.0;
        Yr[x] = 0.0;
      }
    }
  }

  cv::Mat weight;
  GaussianBlur(eyeROI, weight, cv::Size(5, 5), 0, 0);
  for (int y = 0; y < weight.rows; ++y) {
    unsigned char *row = weight.ptr<unsigned char>(y);
    for (int x = 0; x < weight.cols; ++x) {
      row[x] = 255 - row[x];
    }
  }
  cv::Mat outSum = cv::Mat::zeros(eyeROI.rows, eyeROI.cols, CV_64F);
  for (int y = 0; y < weight.rows; ++y) {
    const double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
    for (int x = 0; x < weight.cols; ++x) {
      double gX = Xr[x], gY = Yr[x];
      if (gX == 0.0 && gY == 0.0) {
        continue;
      }
      test_possible_centers_formula(x, y, weight, gX, gY, outSum);
    }
  }
  // scale all the values down, basically averaging them
  double numGradients = weight.rows * weight.cols;
  cv::Mat out;
  outSum.convertTo(out, CV_64F, 1.0 / numGradients);
  // -- Find the maximum point
  cv::Point maxP;
  double maxVal;
  cv::minMaxLoc(out, nullptr, &maxVal, nullptr, &maxP);
  return unscale_point(maxP, eye, fast_eye_width);
}

}  // namespace eyelike

namespace util {

cv::Rect dlib_to_cv(dlib::rectangle to_convert) {
  return cv::Rect(to_convert.left(), to_convert.top(),
                  to_convert.width(), to_convert.height());
}

dlib::point cv_to_dlib(cv::Point to_convert) {
  return {to_convert.x, to_convert.y};
}
dlib::rectangle cv_to_dlib(cv::Rect to_convert) {
  return {to_convert.x, to_convert.y,
          to_convert.width, to_convert.height};
}

cv::Rect get_eye_region(int eye,
                        dlib::full_object_detection object_detection) {
  int index_ex;
  int index_en;
  if (object_detection.num_parts() == 5) {
    if (eye == 0) {
      index_ex = 0;
      index_en = 1;
    } else {
      index_ex = 2;
      index_en = 3;
    }
  } else {
    if (eye == 0) {
      index_ex = 45;
      index_en = 42;
    } else {
      index_ex = 36;
      index_en = 39;
    }
  }

  auto get_rectangle = [](dlib::point one, dlib::point two)
    -> dlib::rectangle {
      dlib::rectangle result(one, two);
      double scale = (one - two).length() * 1.5;
      return dlib::centered_rect(result, scale, scale);
    };

  return dlib_to_cv(get_rectangle(object_detection.part(index_ex),
                                  object_detection.part(index_en)));
}

}  // namespace util

namespace pipeline {

EyeLike::EyeLike()
    : relative_threshold_factor(0.3),
      sigma_factor(0.005) {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "EyeLike";

  if (config["relative_threshold"]) {
    this->relative_threshold_factor =
      config["relative_threshold"]
        .as<decltype(this->relative_threshold_factor)>();
  }
  if (config["sigma_factor"]) {
    this->sigma_factor =
      config["sigma_factor"].as<decltype(this->sigma_factor)>();
  }
}

void EyeLike::process(util::Data& data) {
  if (data.landmarks.num_parts() < 5) {
    return;
  }

  dlib::rectangle face = data.landmarks.get_rect();

  // avoid OpenCV ROI errors since dlib's bounding box can be outside
  face = face.intersect(
      dlib::rectangle(0, 0,
        data.source_image.cols - 1, data.source_image.rows - 1));

  cv::Mat face_mat = data.source_image(util::dlib_to_cv(face));
  cv::cvtColor(face_mat, face_mat, cv::COLOR_RGB2GRAY);

  for (int i = 0; i < 2; ++i) {
    cv::Rect eye = util::get_eye_region(i, data.landmarks);
    eye.x -= face.left();
    eye.y -= face.top();
    cv::Point pupil = eyelike::find_eye_center(face_mat, eye,
        this->relative_threshold_factor, 50);
    // change 50 to eye size

    data.centers[i] = util::cv_to_dlib(pupil);
  }
}

void EyeLike::visualize(util::Data& data) {
  if (data.landmarks.num_parts() < 5) {
    return;
  }
  this->process(data);

  // define offsets
  dlib::point offset_x(5, 0);
  dlib::point offset_y(0, 5);

  dlib::extract_image_chips(data.image,
      util::get_eyes_chip_details(data.landmarks), data.eyes);


  // assign eyes to new images
  dlib::array<dlib::array2d<dlib::bgr_pixel>> eyes(3);
  dlib::assign_image(eyes[2], data.image);
  for (auto i = decltype(data.eyes.size()){0};
       i < data.eyes.size(); ++i) {
    dlib::rectangle eye = util::cv_to_dlib(util::get_eye_region(i,
          data.landmarks));
    dlib::assign_image(eyes[i], data.eyes[i]);

    dlib::draw_line(eyes[i],
                    data.centers[i] + offset_x,
                    data.centers[i] - offset_x,
                    dlib::rgb_pixel(255, 0, 0));
    dlib::draw_line(eyes[i],
                    data.centers[i] + offset_y,
                    data.centers[i] - offset_y,
                    dlib::rgb_pixel(255, 0, 0));

    dlib::draw_line(eyes[2],
                    data.centers[i] + offset_x + eye.tl_corner(),
                    data.centers[i] - offset_x + eye.tl_corner(),
                    dlib::rgb_pixel(255, 0, 0));
    dlib::draw_line(eyes[2],
                    data.centers[i] + offset_y + eye.tl_corner(),
                    data.centers[i] - offset_y + eye.tl_corner(),
                    dlib::rgb_pixel(255, 0, 0));
  }
  dlib::resize_image(0.5, eyes[2]);

  this->widget->set_image(dlib::tile_images(eyes));
}

}  // namespace pipeline

}  // namespace gaze
