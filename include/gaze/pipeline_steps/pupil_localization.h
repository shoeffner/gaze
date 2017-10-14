// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_PUPIL_LOCALIZATION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_PUPIL_LOCALIZATION_H_

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class PupilLocalization pupil_localization.h
 *        "gaze/pipeline_steps/pupil_localization.h"
 * @brief Implements Timm and Barth (2011) @cite timm2011 to detect eye
 *        centers.
 *
 * // TODO(shoeffner): Write documentation for PupilLocalization.
 */
class PupilLocalization : public PipelineStep {
 public:
    /**
     */
    PupilLocalization();

    /**
     * Detects the eye center for each eye.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    virtual void process(util::Data& data);

    /**
     * @param data The data object to be updated.
     */
    virtual void visualize(util::Data& data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_PUPIL_LOCALIZATION_H_
