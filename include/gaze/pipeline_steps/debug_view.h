// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_DEBUG_VIEW_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_DEBUG_VIEW_H_

#include <string>

#include "opencv2/core.hpp"

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class DebugView debug_view.h "gaze/pipeline_steps/debug_view.h"
 * @brief Shows data in a cv::namedWindow as retrieved by a data accessor.
 *
 * This PipelineStep can be used whenever something should be debugged.
 * It can be initialized with a data accessor (see DebugView::DebugView()).
 *
 * @note Passes the input unchanged on. Meta data will still be added.
 */
class DebugView : public PipelineStep {
  std::function<cv::Mat(util::Data*)> accessor;
  static int counter;
  std::string window_name;

 public:
    /**
     * Determines what to view. Initializes the cv::namedWindow, called `Debug
     * View #` where `#` is an incrementing number per DebugView PipelineStep.
     *
     * @param accessor The accessor. This function determines how to access the
     *                 cv::Mat from the util::Data object.
     */
    explicit DebugView(
        const std::function<cv::Mat(util::Data*)> accessor =
          [](util::Data* data) -> cv::Mat { return data->source_image; });

    /**
     * Shows the accessed data in the `Debug View #` cv::namedWindow.
     *
     * @param data The data object to be updated.
     * @returns The unchanged data object.
     */
    virtual void process(util::Data* data);

    /**
     * Destroys the cv::namedWindow.
     */
    virtual ~DebugView();
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_DEBUG_VIEW_H_
