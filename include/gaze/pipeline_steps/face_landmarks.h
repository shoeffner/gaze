// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_FACE_LANDMARKS_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_FACE_LANDMARKS_H_

#include "dlib/image_processing.h"
#include "dlib/image_processing/frontal_face_detector.h"

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class FaceLandmarks face_landmarks.h
 *        "gaze/pipeline_steps/face_landmarks.h"
 * @brief Uses dlibs pretrained models to detect face landmarks.
 *
 * // TODO(shoeffner): Write documentation for FaceLandmarks.
 */
class FaceLandmarks : public PipelineStep {
  dlib::frontal_face_detector face_detector;
  dlib::shape_predictor shape_predictor;

 public:
    /**
     */
    FaceLandmarks();

    /**
     * Detects face landmarks inside the source image.
     *
     * Updates both eyes and face bounding boxes of data.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    virtual void process(util::Data* data);

    /**
     * @param data The data object to be updated.
     */
    virtual void visualize(util::Data* data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_FACE_LANDMARKS_H_
