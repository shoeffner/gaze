// Copyright 2017 Sebastian Höffner

#include "where_people_look/config.h"

#include "gtk/gtk.h"


namespace wpl {

bool WPLConfig::assistant_data_complete() {
  if (subject_id.empty()
      ||result_dir_path.empty()
      ||stimuli_dir_path.empty()) {
    return false;
  }
  return true;
}

}  // namespace wpl
