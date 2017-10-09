// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_SOURCE_CAPTURE_H_
#define INCLUDE_GAZE_SOURCE_CAPTURE_H_

#include <atomic>
#include <memory>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include "gaze/util/data.h"
#include "gaze/util/spsc_deque.h"


namespace gaze {

/**
 * @class SourceCapture source_capture.h "gaze/source_capture.h"
 * @brief Puts each frame from a source into a util::SPSCDeque for further
 *        processing.
 *
 * An instance of this class should be used inside a std::thread, e.g. like
 * this:
 *
 * @code{.cpp}
 * std::thread(std::ref(*source_capture), std::ref(deque));
 * @endcode
 *
 * The deque can then be used for further processing. To get the latest frame,
 * util::SPSCDeque::back should be used, for the oldest unconsumed
 * util::SPSCDeque::front.
 */
class SourceCapture {
  cv::Mat empty_frame;
  int fps;
  int height;
  std::atomic<bool> running;
  std::unique_ptr<cv::VideoCapture> video_capture;
  int width;

  /**
   * Initializes the member variables, e.g. the empty frame and widths and heights.
   */
  const void init();

 public:
    /** @name Creating a source capture
     *
     * To read from a source capture, use the operator()().
     * You can use the deque to either get the latest (util::SPSCDeque::back)
     * or the oldest (util::SPSCDeque::front) frame until they are consumed.
     */
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
    const int get_frames_per_second() const;

    /**
     * @returns the height of the video source's frames.
     */
    const int get_height() const;

    /**
     * @returns the width of the video source's frames.
     */
    const int get_width() const;
    //@}

    /** @name Threading */
    //@{
    /**
     * Appends each frame of the internal cv::VideoCapture to the provided
     * deque. This is done in an endless loop. Use stop() before calling
     * std::thread::join on the thread.
     *
     * @note You should use this function with a std::thread. Don't forget to
     *       stop() the source capture.
     *
     * @param share_deque The deque to share the captured frames.
     */
    const void operator()(util::SPSCDeque<util::Data>* const share_deque);

    /**
     * Signals the thread that it should stop reading images. Allows the thread
     * to be properly joined.
     */
    const void stop();
    //@}
};

}  // namespace gaze

#endif  // INCLUDE_GAZE_SOURCE_CAPTURE_H_
