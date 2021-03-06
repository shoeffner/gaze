#include "gaze/util/data.h"

#include <utility>


namespace gaze {

namespace util {

Data::Data()
    : eyes(2),
      centers(2),
      pupils(2),
      gaze_points(2),
      execution_times() {
}

Data::Data(const Data& data)
    : source_image(data.source_image),
      landmarks(data.landmarks),
      eyes(2),
      centers(2),
      head_rotation(data.head_rotation),
      head_translation(data.head_translation),
      pupils(2),
      gaze_points(2),
      estimated_gaze_point(data.estimated_gaze_point),
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

  for (auto i = decltype(data.pupils.size()){0};
       i < data.pupils.size(); ++i) {
    this->pupils[i] = data.pupils[i];
  }

  for (auto i = decltype(data.gaze_points.size()){0};
       i < data.gaze_points.size(); ++i) {
    this->gaze_points[i] = data.gaze_points[i];
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
  std::swap(this->pupils, data.pupils);
  std::swap(this->gaze_points, data.gaze_points);
  std::swap(this->estimated_gaze_point, data.estimated_gaze_point);
  std::swap(this->execution_times, data.execution_times);
}

}  // namespace util

}  // namespace gaze
