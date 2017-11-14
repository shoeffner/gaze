// Copyright 2017 Sebastian Höffner

#include "where_people_look/config.h"

#include <string>

#include "boost/filesystem.hpp"


namespace wpl {

bool Config::assistant_data_complete() const {
  boost::filesystem::path initial_path = boost::filesystem::initial_path();
  if (subject_id.empty()
      ||stimuli_dir_path == initial_path) {
    return false;
  }
  return true;
}

const boost::filesystem::path Config::get_stimuli_dir_path() const {
  return this->stimuli_dir_path;
}

const std::string Config::get_subject_id() const {
  return this->subject_id;
}

void Config::set_stimuli_dir_path(const std::string path) {
  boost::filesystem::path full_path = boost::filesystem::system_complete(path);
  this->set_stimuli_dir_path(full_path);
}

void Config::set_stimuli_dir_path(const boost::filesystem::path path) {
  boost::filesystem::path full_path = boost::filesystem::system_complete(path);
  this->stimuli_dir_path = full_path;
}

void Config::set_subject_id(const std::string id) {
  this->subject_id = id;
}

}  // namespace wpl
