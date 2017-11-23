// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_TESTS_DOCTEST_HELPER_H_
#define INCLUDE_GAZE_TESTS_DOCTEST_HELPER_H_

#include <string>
#include <vector>


/**
 * DOCTEST_VALUE_PAREMTERIZED_DATA copied from
 * https://github.com/onqtam/doctest/blob/79a379827251cd819c5286070834ccd0ac628af9/doc/markdown/parameterized-tests.md
 *
 * Allows for parameterized tests.
 */
#define DOCTEST_VALUE_PARAMETERIZED_DATA(data, data_array)                    \
    static std::vector<std::string> _doctest_subcases = [&data_array]() {     \
        std::vector<std::string> out;                                         \
        while (out.size() != data_array.size())                               \
            out.push_back(std::string(#data_array "[") +                      \
                          std::to_string(out.size() + 1) + "]");              \
        return out;                                                           \
    }();                                                                      \
    int _doctest_subcase_idx = 0;                                             \
    std::for_each(data_array.begin(), data_array.end(), [&](const auto& in) { \
        DOCTEST_SUBCASE(_doctest_subcases[_doctest_subcase_idx++].c_str()) {  \
          data = in;                                                          \
        }                                                                     \
    })


#endif  // INCLUDE_GAZE_TESTS_DOCTEST_HELPER_H_
