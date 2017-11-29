#include "gaze/util/data.h"

#include <utility>


namespace gaze {

namespace util {

Data::Data()
    : eyes(2),
      centers(2),
      execution_times() {
}

Data::Data(const Data& data)
    : source_image(data.source_image),
      landmarks(data.landmarks),
      eyes(2),
      centers(2),
      head_rotation(data.head_rotation),
      head_translation(data.head_translation),
      execution_times(data.execution_times) {
  dlib::assign_image(this->image, data.image);

  for (auto i = decltype(data.eyes.size()){0};
       i < data.eyes.size(); ++i) {
    dlib::assign_image(this->eyes[i], data.eyes[i]);
  }
  for (auto i = decltype(data.centers.size()){0};
       i < data.centers.size(); ++i) {
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
  std::swap(this->head_rotation, data.head_rotation);
  std::swap(this->head_translation, data.head_translation);
  std::swap(this->execution_times, data.execution_times);
}

}  // namespace util

}  // namespace gaze
