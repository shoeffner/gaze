#include "gaze/pipeline_steps/fallback_step.h"

#include <string>

#include "yaml-cpp/yaml.h"

#include "gaze/util/config.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

FallbackStep::FallbackStep() {
  YAML::Node config = util::get_config(this->number);
  this->name = config["name"] ?
    config["name"].as<std::string>() : "Fallback Step";
  YAML::Emitter emitter;
  emitter << config;
  this->config = emitter.c_str();
}

void FallbackStep::process(util::Data&) {
}

void FallbackStep::visualize(util::Data&) {
  std::string text(""
      "This is a fallback in case something goes wrong.\n"
      "If you see this, then either your configuration is wrong (see below)\n"
      "or you forgot to implement one place where pipeline steps have to go.\n"
      "\nThis is your configuration:\n\n"
      + this->config +
      "\n\nAnd here is a checklist of what to do to develop a new step:\n"
      "- Create include/gaze/pipeline_steps/....h\n"
      "- Create src/gaze/pipeline_steps/....cpp\n"
      "- Add an #include to include/gaze/pipeline_steps.h\n"
      "- Add the *.cpp file to src/gaze/CMakeLists.txt\n"
      "- Add a case to init_pipeline in src/gaze/gaze_tracker.cpp\n"
      "- If you use a new widget, add traits to "
      "include/gaze/gui/visualizeable.h");
  this->widget->set_text(text);
}

}  // namespace pipeline

}  // namespace gaze
