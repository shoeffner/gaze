// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_

#include <string>

#include "boost/filesystem.hpp"


namespace wpl {

/**
 * @class Config config.h "where_people_look/config.h"
 * @brief Holds all configuration options for the experiment.
 */
class Config {
  std::string subject_id;

  boost::filesystem::path stimuli_dir_path = boost::filesystem::initial_path();

 public:
  /**
   * Checks if the data fromt the wizard at program start is complete.
   * This function can be used to set the completed flag for the assistant.
   *
   * @returns true if all information is provided.
   */
  bool assistant_data_complete() const;

  /** @name Getters */
  //@{
  const boost::filesystem::path get_stimuli_dir_path() const;

  const std::string get_subject_id() const;
  //@}

  /** @name Setters
   *
   * Some setters are overloaded to provide easy usage with
   * boost::filesystem::path and std::string.
   */
  //@{
  void set_stimuli_dir_path(const std::string path);

  void set_stimuli_dir_path(const boost::filesystem::path path);

  void set_subject_id(const std::string id);
  //@}
};

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_CONFIG_H_
