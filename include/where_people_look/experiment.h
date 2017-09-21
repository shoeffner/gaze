// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
#define INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_

#include "where_people_look/config.h"


namespace wpl {

class Experiment {
  WPLConfig* config;

 public:
    explicit Experiment(WPLConfig*);
};

}  // namespace wpl

#endif  // INCLUDE_WHERE_PEOPLE_LOOK_EXPERIMENT_H_
