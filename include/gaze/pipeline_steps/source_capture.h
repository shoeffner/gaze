// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_SOURCE_CAPTURE_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_SOURCE_CAPTURE_H_

#include <memory>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class SourceCapture source_capture.h "gaze/pipeline_steps/source_capture.h"
 * @brief Retrieves frames from a webcam or video source.
 *
 * Should be used at the beginning of a Pipeline.
 */
class SourceCapture final
    : public PipelineStep,
      public gui::ImageVisualizeable {
  cv::VideoCapture video_capture;
  cv::Mat last_frame;

 public:
    /** @name Creating a source capture */
    //@{
    /**
     * Creates a cv::VideoCapture as the input source.
     *
     * If the source specified in the yaml configuration is a string, it is
     * considered as a path, otherwise the number will be parsed as a webcam
     * address.
     */
    SourceCapture();
    //@}

    ~SourceCapture();

 public:
    /**
     * Reads a frame from the video source and stores it as the data's
     * util::Data::souce_image.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    void process(util::Data& data) override;

    /**
     * Just assigns the data's raw camera image
     * to the widget.
     *
     * @param data the data object.
     */
    void visualize(util::Data& data) override;
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_SOURCE_CAPTURE_H_
