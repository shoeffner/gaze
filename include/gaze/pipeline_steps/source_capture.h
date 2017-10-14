// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_SOURCE_CAPTURE_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_SOURCE_CAPTURE_H_

#include <memory>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class SourceCapture source_capture.h "gaze/pipeline_steps/source_capture.h"
 * @brief Puts each frame from a source into a util::SPSCDeque for further
 *        processing.
 *
 * Should be used at the beginning of a Pipeline.
 */
class SourceCapture : public PipelineStep {
  cv::Mat empty_frame;
  cv::VideoCapture video_capture;

 public:
    /** @name Creating a source capture */
    //@{
    /**
     * Creates a cv::VideoCapture with a webcam source.
     *
     * Tries to set the FPS to 60, however most webcams only can capture about
     * 30 FPS. Use get_frames_per_second() to figure out how many frames your
     * source eventually has.
     *
     * @param source The webcam id, usually 0.
     */
    explicit SourceCapture(const int source);

    /**
     * Creates a cv::VideoCapture with a video source.
     *
     * @param source The video source file path.
     */
    explicit SourceCapture(const std::string source);
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
    virtual void process(util::Data& data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_SOURCE_CAPTURE_H_
