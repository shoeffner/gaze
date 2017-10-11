// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/into_the_void_step.h"

#include "gaze/util/data.h"
#include "gaze/util/spsc_deque.h"


namespace gaze {

namespace pipeline {

const void IntoTheVoidStep::store_data(
    util::SPSCDeque<util::Data>* const out_deque,
    const util::Data data) const {
}

void IntoTheVoidStep::process(util::Data* data) {
}

}  // namespace pipeline

}  // namespace gaze
