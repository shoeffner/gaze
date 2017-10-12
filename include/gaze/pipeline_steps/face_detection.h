// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_FACE_DETECTION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_FACE_DETECTION_H_

#include "opencv2/objdetect.hpp"

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class FaceDetection face_detection.h
 *        "gaze/pipeline_steps/face_detection.h"
 * @brief Uses OpenCV's cv::CascadeClassifier with pretrained haarcascades to
 *        detect faces.
 *
 * // TODO(shoeffner): Write documentation for FaceDetection.
 */
class FaceDetection : public PipelineStep {
  cv::CascadeClassifier frontal_face_classifier;
  cv::CascadeClassifier profile_face_classifier;

 public:
    /**
     * Loads the haarcascades file.
     */
    FaceDetection();

    /**
     * Detects faces inside the source image using two haarcascades.
     * First looks for frontal faces, if none is found falls back
     * to profile faces.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    virtual void process(util::Data* data);

    /**
     * Draws a blue rectangle around the detected face.
     *
     * @param data The data object to be updated.
     */
    virtual void visualize(util::Data* data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_FACE_DETECTION_H_
