// Copyright 2017 Sebastian Höffner

#include "gaze/pipeline_steps/pupil_localization.h"

#include <fstream>
#include <iomanip>
#include <list>
#include <numeric>
#include <string>
#include <vector>

#include "dlib/matrix.h"
#include "doctest/doctest.h"
#include "opencv2/opencv.hpp"

#include "gaze/pipeline_steps/face_landmarks.h"
#include "gaze/util/data.h"
#include "gaze/tests/doctest_helper.h"


template<typename T>
void CHECK_MATRIX_APPROX(dlib::matrix<T> expected,
                         dlib::matrix<T> actual,
                         T a) {
  for (auto i = decltype(expected.nr()){0}; i < expected.nr(); ++i) {
    for (auto j = decltype(expected.nc()){0}; j < expected.nc(); ++j) {
      CHECK(actual(i, j) == doctest::Approx(expected(i, j)).epsilon(a));
    }
  }
}

TEST_CASE("gaze::util::fill_displacement_tables") {
  dlib::matrix<double> table_x;
  dlib::matrix<double> table_y;

  SUBCASE("empty matrices, new size 3") {
    dlib::matrix<double, 3, 3> table_expected;
    table_expected = -0.707, 0, 0.707,
                     -1,     0, 1,
                     -0.707, 0, 0.707;

    gaze::util::fill_displacement_tables(table_x, table_y, 3);

    REQUIRE(table_x.nr() == 3);
    REQUIRE(table_x.nc() == 3);

    REQUIRE(table_y.nr() == 3);
    REQUIRE(table_y.nc() == 3);

    CHECK_MATRIX_APPROX<double>(table_expected, table_x, 0.001);
    CHECK_MATRIX_APPROX<double>(dlib::trans(table_expected), table_y, 0.001);
  }

  table_x.set_size(3, 3);
  table_y.set_size(3, 3);

  SUBCASE("filled 3x3 matrices, same size 3") {
    table_x = 1, 2, 3, 4, 5, 6, 7, 8, 9;
    table_y = dlib::trans(table_x);
    dlib::matrix<double> table_expected(table_x);

    gaze::util::fill_displacement_tables(table_x, table_y, 3);

    CHECK(table_x == table_expected);
    CHECK(table_y == dlib::trans(table_expected));
  }

  SUBCASE("expand size") {
    dlib::matrix<double, 5, 5> table_expected;
    table_expected = -0.707, -0.447, 0, 0.447, 0.707,
                     -0.894, -0.707, 0, 0.707, 0.894,
                     -1,     -1,     0, 1,     1,
                     -0.894, -0.707, 0, 0.707, 0.894,
                     -0.707, -0.447, 0, 0.447, 0.707;

    SUBCASE("filled 3x3 matrices, new size 4") {
      gaze::util::fill_displacement_tables(table_x, table_y, 4);

      CHECK_MATRIX_APPROX<double>(table_expected, table_x, 0.001);
      CHECK_MATRIX_APPROX<double>(dlib::trans(table_expected), table_y, 0.001);
    }

    SUBCASE("filled 3x3 matrices, new size 5") {
      gaze::util::fill_displacement_tables(table_x, table_y, 5);

      CHECK_MATRIX_APPROX<double>(table_expected, table_x, 0.001);
      CHECK_MATRIX_APPROX<double>(dlib::trans(table_expected), table_y, 0.001);
    }
  }
}


// This test is allowed to fail, as several images are just not properly
// detected by the eye center localization algorithm.
// Specifically, images
// 9, 14, 50, 52, 62, 63, 64, 70, 71, and 104
// are known to fail.
TEST_CASE("PupilLocalization::process" * doctest::may_fail()) {
  // SHARED TEST SETUP
  std::string FACE;
  // available test images
  std::list<int> IMAGE_NUMS(120);  // 120

  // Skipping tests if IMAGE_NUMS(0)
  if (IMAGE_NUMS.size() == 0) {
    return;
  }

  std::iota(IMAGE_NUMS.begin(), IMAGE_NUMS.end(), 0);
  std::list<std::string> IMAGES;
  for (auto num : IMAGE_NUMS) {
    std::stringstream s;
    s << std::setfill('0') << std::setw(4) << num;
    IMAGES.push_back(s.str());
  }

  double TOLERANCE = 15.0 / 60.0;  // roughly 15x15 pixel error for 60px eye
                                   // (this is a lot)
  INFO("Tolerance: " << TOLERANCE);
  std::string PATH("./assets/pexels_faces/");
  std::string COMMA(", ");  // For crude csv parsing


  // Pipeline steps (preparation and tested)
  gaze::pipeline::FaceLandmarks landmarks;
  gaze::pipeline::PupilLocalization pupil_localization;

  int target_x[] = {0, 0};
  int target_y[] = {0, 0};

  // INDIVIDUAL TEST SETUP
  DOCTEST_VALUE_PARAMETERIZED_DATA(FACE, IMAGES);
  INFO("Face: " << FACE);

  // Read and parse target values
  std::ifstream target_reader(PATH + FACE + ".csv");
  REQUIRE(target_reader.is_open());
  target_reader >> target_x[0] >> COMMA
                >> target_y[0]
                >> target_x[1] >> COMMA
                >> target_y[1];
  // Invalid test case (no face annotations)
  if (target_x[0] + target_y[0] + target_x[1] + target_y[1] == 0) {
    return;
  }

  // Create data object and load test image
  gaze::util::Data data;
  data.source_image = cv::imread(
      PATH + FACE + ".jpeg");
  dlib::assign_image(
      data.image,
      dlib::cv_image<dlib::bgr_pixel>(data.source_image));

  // Extract landmark information
  landmarks.process(data);
  if (data.landmarks.num_parts() == 0) {
    return;
  }
  std::vector<dlib::chip_details> details =
    gaze::util::get_eyes_chip_details(data.landmarks);

  // ACTUAL TEST
  pupil_localization.process(data);
  for (int i = 0; i < 2; ++i) {
    int x = target_x[i] - details[i].rect.left();
    int y = target_y[i] - details[i].rect.top();
    CHECK(data.centers[i].x() == doctest::Approx(x).epsilon(TOLERANCE));
    CHECK(data.centers[i].y() == doctest::Approx(y).epsilon(TOLERANCE));
  }

  // TEARDOWN
  // Store result for easy debugging of failing test results
  for (int i = 0; i < 2; ++i) {
    cv::drawMarker(
      data.source_image,
      cv::Point(target_x[i], target_y[i]),
      cv::Scalar(0, 255, 0));
    cv::drawMarker(
      data.source_image,
      cv::Point(details[i].rect.left() + data.centers[i].x(),
                details[i].rect.top() + data.centers[i].y()),
      cv::Scalar(0, 0, 255));
  }
  cv::imwrite(PATH + FACE + "_out.jpeg", data.source_image);
}
