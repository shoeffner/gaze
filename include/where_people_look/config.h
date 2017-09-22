// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_

#include <string>

#include "boost/filesystem.hpp"


namespace wpl {

class Config {
  std::string subject_id;

  boost::filesystem::path result_dir_path = boost::filesystem::initial_path();
  boost::filesystem::path stimuli_dir_path = boost::filesystem::initial_path();

 public:
  const bool assistant_data_complete() const;

  const boost::filesystem::path get_result_dir_path() const;

  const boost::filesystem::path get_stimuli_dir_path() const;

  const std::string get_subject_id() const;

  const void set_result_dir_path(const std::string path);

  const void set_result_dir_path(const boost::filesystem::path path);

  const void set_stimuli_dir_path(const std::string path);

  const void set_stimuli_dir_path(const boost::filesystem::path path);

  const void set_subject_id(const std::string id);
};

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_
