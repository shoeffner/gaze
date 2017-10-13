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
 */
class EyeDetection : public PipelineStep {
  cv::CascadeClassifier eye_classifier;

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
     * Draws a green rectangle around the detected eyes.
     *
     * @param data The data object to be updated.
     */
    virtual void visualize(util::Data* data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_EYE_DETECTION_H_
