#include "gaze/util/pipeline_utils.h"

#include <vector>

#include "dlib/image_transforms.h"


namespace gaze {

namespace util {

std::vector<dlib::chip_details> get_eyes_chip_details(
    const dlib::full_object_detection object_detection) {
  std::vector<dlib::chip_details> details;
  if (object_detection.num_parts() < 5) {
    return details;
  }

  auto get_rectangle = [](dlib::point one, dlib::point two)
    -> dlib::rectangle {
      dlib::rectangle result(one, two);
      double scale = (one - two).length() * 1.5;
      return dlib::centered_rect(result, scale, scale);
    };

  int index_ex_left;
  int index_en_left;
  int index_ex_right;
  int index_en_right;
  if (object_detection.num_parts() == 5) {
    index_ex_left = 2;
    index_en_left = 3;
    index_ex_right = 0;
    index_en_right = 1;
  } else {  // 68 landmarks
    index_ex_left = 45;
    index_en_left = 42;
    index_ex_right = 36;
    index_en_right = 39;
  }

  details.push_back(dlib::chip_details(
        get_rectangle(object_detection.part(index_ex_right),
                      object_detection.part(index_en_right))));
  details.push_back(dlib::chip_details(
        get_rectangle(object_detection.part(index_ex_left),
                      object_detection.part(index_en_left))));

  return details;
}

}  // namespace util

}  // namespace gaze
