// Copyright 2017 Sebastian Höffner

#include "gaze/util/data.h"

#include <utility>

#include <iostream>
namespace gaze {

namespace util {

Data::Data()
    : eyes(2),
      centers(2) {
}

Data::Data(Data& data)
    : source_image(data.source_image),
      landmarks(data.landmarks),
      eyes(2),
      centers(2),
      execution_times() {
  dlib::assign_image(this->image, data.image);

  for (unsigned long i = 0; i < data.eyes.size(); ++i) {
    dlib::assign_image(this->eyes[i], data.eyes[i]);
  }
  for (unsigned long i = 0; i < data.centers.size(); ++i) {
    this->centers[i] = data.centers[i];
  }
}

Data& Data::operator=(Data data) {
  this->swap(data);
  return *this;
}

void Data::swap(Data& data) {
  std::swap(this->source_image, data.source_image);
  std::swap(this->image, data.image);
  std::swap(this->landmarks, data.landmarks);
  std::swap(this->eyes, data.eyes);
  std::swap(this->centers, data.centers);
}

}  // namespace util

}  // namespace gaze
