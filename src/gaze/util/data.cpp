// Copyright 2017 Sebastian Höffner

#include "gaze/util/data.h"

#include <utility>


namespace gaze {

namespace util {

Data::Data() : eyes(2) {
}

Data::Data(Data& data)
    : source_image(data.source_image),
      landmarks(data.landmarks),
      eyes(2) {
  dlib::assign_image(this->image, data.image);
  for (dlib::array2d<dlib::bgr_pixel>& eye : eyes) {
    this->eyes.push_back(eye);
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
}

}  // namespace util

}  // namespace gaze
