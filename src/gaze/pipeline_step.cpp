// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_step.h"

#include <string>

#include "opencv2/imgproc.hpp"

namespace gaze {

const int PipelineStep::FONT = cv::HersheyFonts::FONT_HERSHEY_DUPLEX;
const double PipelineStep::FONT_SCALE = 0.6;
const int PipelineStep::FONT_THICKNESS = 1;
const int PipelineStep::FONT_BASELINE = 0;

int PipelineStep::counter = 0;
int PipelineStep::font_height = 0;

const void PipelineStep::write_text(cv::Mat* image,
                                    const std::string text) const {
  int offset_x = 10;
  int offset_y = 5 + this->number * (PipelineStep::font_height + 3);
  // First write black "shadow", then overwrite with white
  cv::putText(*image,
              text,
              cv::Point(offset_x + 1, offset_y + 1),
              PipelineStep::FONT,
              PipelineStep::FONT_SCALE,
              cv::Scalar(0, 0, 0),
              PipelineStep::FONT_THICKNESS);
  cv::putText(*image,
              text,
              cv::Point(offset_x, offset_y),
              PipelineStep::FONT,
              PipelineStep::FONT_SCALE,
              cv::Scalar(255, 255, 255),
              PipelineStep::FONT_THICKNESS);
}

PipelineStep::PipelineStep() {
  this->number = PipelineStep::counter++;

  int baseline = PipelineStep::FONT_BASELINE;
  PipelineStep::font_height = cv::getTextSize("Ij",  // some "high" text
                                              PipelineStep::FONT,
                                              PipelineStep::FONT_SCALE,
                                              PipelineStep::FONT_THICKNESS,
                                              &baseline).height;
}

void PipelineStep::visualize(util::Data* data) {
  // Do nothing by default.
}

}  // namespace gaze
