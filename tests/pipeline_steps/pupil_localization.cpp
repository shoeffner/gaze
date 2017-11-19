// Copyright 2017 Sebastian Höffner

#include <iostream>

#include "dlib/matrix.h"
#include "doctest/doctest.h"

#include "gaze/pipeline_steps/pupil_localization.h"

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

