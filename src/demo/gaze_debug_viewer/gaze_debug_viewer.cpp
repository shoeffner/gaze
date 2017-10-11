// Copyright 2017 Sebastian Höffner

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "boost/program_options.hpp"

#include "gaze/gaze.h"


namespace {

static boost::program_options::variables_map
parse_program_options(int argc, const char** argv) {
  boost::program_options::options_description
    commandline("Command line arguments");
  commandline.add_options()
    ("help", "Shows this help message")
    ("input_source",
     boost::program_options::value<std::string>()->default_value("0"),
     "The input source. An integer for webcam access (usually 0), or a string "
     "as a path to a video file.")
  ;  // NOLINT

  boost::program_options::positional_options_description positional;
  positional.add("input_source", 1);

  // Create variable map
  std::shared_ptr<boost::program_options::variables_map> map(
    new boost::program_options::variables_map());

  // Parse command line arguments
  boost::program_options::store(
      boost::program_options::command_line_parser(argc, argv)
        .options(commandline).positional(positional).run(),
      *map);

  boost::program_options::notify(*map);

  if (map->count("help")) {
    std::cout << commandline << std::endl;
    std::exit(0);
  }

  return *map;
}

void register_exit_handlers() {
  for (int signal : {SIGINT, SIGTERM}) {
    std::signal(signal, [](int) -> void { std::exit(0); });
  }
}

}  // namespace


int main(int argc, const char** argv) {
  ::register_exit_handlers();
  boost::program_options::variables_map options =
    ::parse_program_options(argc, argv);

  std::string input_source = options["input_source"].as<std::string>();

  std::unique_ptr<gaze::GazeTracker> gaze_tracker(
      new gaze::GazeTracker(input_source));

  // gaze_tracker->print_capture_info();
  // gaze_tracker->show_debug_screen();
}
