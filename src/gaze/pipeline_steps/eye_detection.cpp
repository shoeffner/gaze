// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/eye_detection.h"

#include <string>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include "gaze/util/data.h"
#include "gaze/util/utility.h"


namespace gaze {

namespace pipeline {


EyeDetection::EyeDetection() {
  std::string eye_path("haarcascade_eye_tree_eyeglasses.xml");
  this->eye_classifier = util::load_classifier(eye_path);
}

const void EyeDetection::assign_left_eye(util::Data* const data,
                                         const cv::Rect left_eye) {
  data->left_eye_found = true;
  data->left_eye = left_eye;
  this->last_left_eye = left_eye;
}

const void EyeDetection::assign_right_eye(util::Data* const data,
                                          const cv::Rect right_eye) {
  data->right_eye_found = true;
  data->right_eye = right_eye;
  this->last_right_eye = right_eye;
}

void EyeDetection::process(util::Data* data) {
  if (!data->face_found) {
    this->last_left_eye = cv::Rect();
    this->last_right_eye = cv::Rect();
    return;
  }

  // Detect eyes inside face
  std::vector<cv::Rect> detections;
  this->eye_classifier.detectMultiScale(
      data->source_image(data->face), detections, 1.1, 3, 0,
      // Note: Arbitrary guessed heuristics
      cv::Size(data->face.width / 4, data->face.height / 5),
      cv::Size(data->face.width / 2, data->face.height / 3));

  for (cv::Rect& r : detections) {
    r.x += data->face.x;
    r.y += data->face.y;
  }

  // By default use old eyes
  cv::Rect next_left_eye = this->last_left_eye;
  cv::Rect next_right_eye = this->last_right_eye;
  int center = data->face.x + data->face.width / 2;
  switch (detections.size()) {
    case 0:
      break;

    case 1:  // Keep old eyes
      if (util::ltr(detections[0], center)) {
        next_left_eye = detections[0];
      } else {
        next_right_eye = detections[0];
      }
      break;

    case 2:
      // TODO(shoeffner): Might be superfluous if the general case is fast.
      if (util::ltr(detections[0], center, detections[1])) {
        next_left_eye = detections[0];
        next_right_eye = detections[1];
      } else if (util::ltr(detections[1], center, detections[0])) {
        next_left_eye = detections[1];
        next_right_eye = detections[0];
      } else {
        // both are on the same side
        if (util::ltr(detections[0], center)
            && util::ltr(detections[1], center)) {
          if (next_left_eye.empty()) {
            next_left_eye = detections[0];
          } else {
            next_left_eye = util::closest_to_center(next_left_eye, detections);
          }
        } else {
          if (next_right_eye.empty()) {
            next_right_eye = detections[0];
          } else {
            next_right_eye = util::closest_to_center(next_right_eye,
                                                     detections);
          }
        }
      }
      break;

    default:
      // Sort candidates to left and right, pick the one closest to the last
      // detected respective eye. If none was detected, pick the first of each
      // set, respectively.
      std::vector<cv::Rect> left_candidates;
      std::vector<cv::Rect> right_candidates;
      for (cv::Rect r : detections) {
        if (util::ltr(r, center)) {
          left_candidates.push_back(r);
        } else {
          right_candidates.push_back(r);
        }
      }

      if (!left_candidates.empty()) {
        if (next_left_eye.empty()) {
          next_left_eye = left_candidates[0];
        } else {
          next_left_eye = util::closest_to_center(next_left_eye,
                                                  left_candidates);
        }
      }

      if (!right_candidates.empty()) {
        if (next_right_eye.empty()) {
          next_right_eye = right_candidates[0];
        } else {
          next_right_eye = util::closest_to_center(next_right_eye,
                                                   right_candidates);
        }
      }
      break;
  }

  this->assign_left_eye(data, next_left_eye);
  this->assign_right_eye(data, next_right_eye);
}

void EyeDetection::visualize(util::Data* data) {
  int eyes = data->left_eye_found + data->right_eye_found;
  if (eyes) {
    if (data->left_eye_found) {
      cv::rectangle(data->source_image,
                    data->left_eye,
                    cv::Scalar(0, 180, 0));
    }
    if (data->right_eye_found) {
      cv::rectangle(data->source_image,
                    data->right_eye,
                    cv::Scalar(0, 255, 0));
    }
    this->write_text(&data->source_image,
        "Eye detection: " + std::to_string(eyes) + " detected.");
  } else {
    this->write_text(&data->source_image, "Eye detection: None detected.");
  }
}

}  // namespace pipeline

}  // namespace gaze
