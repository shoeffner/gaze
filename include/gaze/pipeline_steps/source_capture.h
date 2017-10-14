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
  int fps;
  int height;
  std::unique_ptr<cv::VideoCapture> video_capture;
  int width;

  /**
   * Initializes the member variables, e.g. the empty frame and widths and heights.
   */
  void init();

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

    /** @name Helper functions
     *
     * In some contexts you might want some additional information about the
     * source. The get_ methods will read the properties from the
     * cv::VideoCapture, while get_empty_frame() returns a cv::Mat of the
     * source size which is filled with black pixels. It can be used
     * as a default value for e.g. util::SPSCDeque::front_or_default().
     */
    //@{
    /**
     * @returns an empty frame, that is a cv::Mat filled with `0`s of size
     *          @f$\mathit{height}\times\mathit{width}@f$.
     */
    const cv::Mat get_empty_frame() const;

    /**
     * @returns The number of frames per second for the internal
     *          cv::VideoCapture source. For videos this will be the value
     *          the video file had, for webcams it will be as high as the
     *          camera allowed, but at most around 60 FPS.
     */
    int get_frames_per_second() const;

    /**
     * @returns the height of the video source's frames.
     */
    int get_height() const;

    /**
     * @returns the width of the video source's frames.
     */
    int get_width() const;
    //@}

 public:
    /**
     * Reads a frame from the video source and stores it as the data's
     * util::Data::souce_image.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    virtual void process(util::Data* data);
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_SOURCE_CAPTURE_H_
