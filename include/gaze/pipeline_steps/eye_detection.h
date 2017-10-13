// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_EYE_DETECTION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_EYE_DETECTION_H_

#include "opencv2/objdetect.hpp"

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class EyeDetection eye_detection.h
 *        "gaze/pipeline_steps/eye_detection.h"
 * @brief Uses OpenCV's cv::CascadeClassifier with pretrained haarcascades to
 *        detect eyes.
 *
 * // TODO(shoeffner): Write documentation for EyeDetection.
 * // TODO(shoeffner): Track last X frames for when no eye can be found?
 *                     Especially interesting are running filters or such -
 *                     same for face! Alternatively check
 *                     https://github.com/trishume/eyeLike/blob/master/src/main.cpp#L98
 */
class EyeDetection : public PipelineStep {
  cv::CascadeClassifier eye_classifier;
  cv::Rect last_left_eye;
  cv::Rect last_right_eye;

  /**
   * Assigns the left eye to the data object and the last left eye.
   *
   * @param data The data object.
   * @param left_eye The eye to assign.
   */
  const void assign_left_eye(util::Data* const data,
                             const cv::Rect left_eye);

  /**
   * Assigns the right eye to the data object and the last right eye.
   *
   * @param data The data object.
   * @param right_eye The eye to assign.
   */
  const void assign_right_eye(util::Data* const data,
                              const cv::Rect right_eye);

 public:
    /**
     * Loads the haarcascades file.
     */
    EyeDetection();

    /**
     * Detects eyes inside previously detected faces.
     * If no face was found previously, this does not nothing.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    virtual void process(util::Data* data);

    /**
     * Draws green rectangles around the detected eyes.
     *
     * @param data The data object to be updated.
     */
    virtual void visualize(util::Data* data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_EYE_DETECTION_H_
