// Copyright 2017 Sebastian Höffner

#include "where_people_look/config.h"


namespace wpl {

const bool Config::assistant_data_complete() const {
  if (subject_id.empty()
      ||result_dir_path.empty()
      ||stimuli_dir_path.empty()) {
    return false;
  }
  return true;
}

}  // namespace wpl
