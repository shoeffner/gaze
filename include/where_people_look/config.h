// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_

#include <string>


namespace wpl {

struct Config {
  const bool assistant_data_complete() const;

  std::string subject_id;

  std::string result_dir_path;

  std::string stimuli_dir_path;
};

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_
