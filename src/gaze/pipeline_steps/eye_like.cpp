#include "gaze/pipeline_steps/eye_like.h"

#include <cmath>

#include "opencv2/opencv.hpp"
#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"
#include "gaze/util/dlibcv.h"
#include "gaze/util/pipeline_utils.h"


namespace gaze {

/**
 * @namespace eyeLike
 * @brief Tristan Hume's EyeLike adapted for gaze.
 *
 * The code in this namespace is mainly taken from Tristan Hume's
 * implementation of Timm and Barth (2011) @cite Timm2011, eyeLike.
 * The original source was released 2013 under the MIT license, which
 * grants permission to copy and redistribute this code.
 * Some adjustments were made to make the code meet the coding principles
 * used throughout the gaze library, some dead code was removed and
 * everything was placed into this namespace to make it easy to find it.
 *
 * For the original source please refer to
 * <a href="https://github.com/trishume/eyeLike">Tristan's GitHub repository</a>.
 */
namespace eyelike {

/**
 * Computes a dynamic threshold to discard low magnitudes.
 *
 * @param mat The matrix of magnitudes.
 * @param std_dev_factor The threshold will be std_dev_factor many standard
 *                       deviations above the mean.
 * @returns The threshold.
 */
double compute_dynamic_threshold(const cv::Mat& mat, double std_dev_factor) {
  cv::Scalar stdMagnGrad, meanMagnGrad;
  cv::meanStdDev(mat, meanMagnGrad, stdMagnGrad);
  double std_dev = stdMagnGrad[0];
  return std_dev_factor * std_dev + meanMagnGrad[0];
}

/**
 * Computes the horizontal gradient of the input matrix.
 *
 * This function is, according to Tristan, similar to the gradient function in
 * Matlab.
 *
 * @param image the input image
 * @returns the gradient matrix
 */
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

/**
 * Calculates the magnitudes of a two dimensional gradient.
 *
 * @param mat_x The gradient in x direction.
 * @param mat_y The gradient in y direction.
 * @returns the magnitude.
 */
cv::Mat matrix_magnitude(const cv::Mat& mat_x, const cv::Mat& mat_y) {
  cv::Mat mags(mat_x.rows, mat_x.cols, CV_64F);
  for (auto y = decltype(mat_x.rows){0}; y < mat_x.rows; ++y) {
    const double *Xr = mat_x.ptr<double>(y), *Yr = mat_y.ptr<double>(y);
    double *row = mags.ptr<double>(y);
    for (auto x = decltype(mat_x.cols){0}; x < mat_x.cols; ++x) {
      double gX = Xr[x], gY = Yr[x];
      double magnitude = std::sqrt((gX * gX) + (gY * gY));
      row[x] = magnitude;
    }
  }
  return mags;
}

/**
 * Evaluates the target function for each possible pupil center.
 *
 * The results are added to the respective positions in the out matrix.
 * As stated in the original code, this is basically changing the order
 * as it is proposed in Timm and Barth (2011) @cite Timm2011, where
 * the loop is over the centers and then gradients, while here it is
 * over (relevant) gradients and all possible centers.
 *
 * @param x The x coordinate (column).
 * @param y The y coordinate (row).
 * @param weight The weight matrix to get the weight for the center point.
 * @param gx The horizontal gradient value.
 * @param gy The veritcal gradient value.
 * @param out The result matrix.
 */
void test_possible_centers_formula(
    int x, int y,
    const cv::Mat& weight,
    double gx, double gy,
    cv::Mat& out) {  // NOLINT
  for (auto cy = decltype(out.rows){0}; cy < out.rows; ++cy) {
    double *out_row = out.ptr<double>(cy);
    const unsigned char *Wr = weight.ptr<unsigned char>(cy);
    for (auto cx = decltype(out.cols){0}; cx < out.cols; ++cx) {
      if (x == cx && y == cy) {
        continue;
      }
      double dx = x - cx;
      double dy = y - cy;
      // normalize d
      double magnitude = std::sqrt(dx * dx + dy * dy);
      dx = dx / magnitude;
      dy = dy / magnitude;
      double dotProduct = dx * gx + dy * gy;
      dotProduct = std::max(0.0, dotProduct);
      out_row[cx] += dotProduct * dotProduct * Wr[cx];
    }
  }
}

/**
 * Transforms a point back to the original eye size.
 *
 * @param point The point to transform.
 * @param orig_size The original size.
 * @param fast_eye_width The scale width.
 * @returns A point which is at the respective position in the original image
 *          as it was in the scaled image.
 */
cv::Point unscale_point(cv::Point point, cv::Rect orig_size,
                        double fast_eye_width) {
  double ratio = fast_eye_width / orig_size.width;
  int x = std::round(point.x / ratio);
  int y = std::round(point.y / ratio);
  return cv::Point(x, y);
}

/**
 * Applies the eye center localization algorithm.
 *
 * @param face The cropped face.
 * @param eye The eye boundary.
 * @param gradient_threshold The value for the adaptive gradient threshold,
 *        used by compute_dynamic_threshold(magnitudes, gradient_threshold)
 * @param fast_eye_width Used to scale the eye to size on which the algorithm
 *        should operate. Default is 50. Used by
 *        unscale_point(point, original_size, fast_eye_width)
 * @returns The estimated eye center point.
 */
cv::Point find_eye_center(
    const cv::Mat& face, const cv::Rect& eye,
    double gradient_threshold, double fast_eye_width = 50) {
  cv::Rect eye_roi_cut = eye;
  eye_roi_cut.width = std::min(eye.width, face.cols - eye.x);
  eye_roi_cut.height = std::min(eye.height, face.rows - eye.y);
  eye_roi_cut.x = std::max(eye.x, 0);
  eye_roi_cut.y = std::max(eye.y, 0);

  cv::Mat eye_roi_unscaled = face(eye_roi_cut);
  cv::Mat eye_roi;
  cv::resize(
      eye_roi_unscaled,
      eye_roi,
      cv::Size(fast_eye_width,
               fast_eye_width / eye_roi_unscaled.cols * eye_roi_unscaled.rows));

  cv::Mat gradientX = compute_x_gradient(eye_roi);
  cv::Mat gradientY = compute_x_gradient(eye_roi.t()).t();
  cv::Mat mags = matrix_magnitude(gradientX, gradientY);
  double gradientThresh = compute_dynamic_threshold(mags,
      gradient_threshold);

  for (auto y = decltype(eye_roi.rows){0}; y < eye_roi.rows; ++y) {
    double *x_row = gradientX.ptr<double>(y);
    double *y_row = gradientY.ptr<double>(y);
    const double *M_ROW = mags.ptr<double>(y);
    for (auto x = decltype(eye_roi.cols){0}; x < eye_roi.cols; ++x) {
      if (M_ROW[x] > gradientThresh) {
        x_row[x] /= M_ROW[x];
        y_row[x] /= M_ROW[x];
      } else {
        x_row[x] = 0.0;
        y_row[x] = 0.0;
      }
    }
  }

  cv::Mat weight;
  cv::GaussianBlur(eye_roi, weight, cv::Size(5, 5), 0, 0);
  weight = 255 - weight;

  cv::Mat outSum = cv::Mat::zeros(eye_roi.rows, eye_roi.cols, CV_64F);
  for (int y = 0; y < weight.rows; ++y) {
    const double *X_ROW = gradientX.ptr<double>(y);
    const double *Y_ROW = gradientY.ptr<double>(y);
    for (int x = 0; x < weight.cols; ++x) {
      if (X_ROW[x] == 0.0 && Y_ROW[x] == 0.0) {
        continue;
      }
      test_possible_centers_formula(x, y, weight, X_ROW[x], Y_ROW[x], outSum);
    }
  }

  cv::Point maxP;
  double maxVal;
  cv::minMaxLoc(outSum, nullptr, &maxVal, nullptr, &maxP);

  return unscale_point(maxP, eye, fast_eye_width);
}

}  // namespace eyelike

namespace util {

/**
 * Returns the bounding box around an eye region.
 * Works for both, the 5 and the 68 landmarks model.
 *
 * @param eye 0 for left eye, 1 for right eye.
 * @param object_detection The detected face landmarks.
 *
 * @returns the bounding box.
 */
dlib::rectangle get_eye_region(int eye,
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

  return get_rectangle(object_detection.part(index_ex),
                       object_detection.part(index_en));
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
    dlib::rectangle eye = util::get_eye_region(i, data.landmarks);
    eye.set_left(eye.left() - face.left());
    eye.set_right(eye.right() - face.left());
    eye.set_top(eye.top() - face.top());
    eye.set_bottom(eye.bottom() - face.top());

    cv::Point pupil = eyelike::find_eye_center(face_mat,
        util::dlib_to_cv(eye),
        this->relative_threshold_factor, 50);

    data.centers[i] = util::cv_to_dlib(pupil);
  }
}

void EyeLike::visualize(util::Data& data) {
  if (data.landmarks.num_parts() < 5) {
    return;
  }
  // Ensures output of this pipeline step is visualized
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
    dlib::rectangle eye = util::get_eye_region(i, data.landmarks);
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
