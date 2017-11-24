// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_PUPIL_LOCALIZATION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_PUPIL_LOCALIZATION_H_

#include <algorithm>  // Not used, but cpplint detects max() as part of this
#include <vector>

#include "dlib/matrix.h"

#include "gaze/gui/visualizeable.h"
#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace util {

/**
 * Determines a bounding box around the eyes from a given object detection.
 * This function is designed to work with 5 feature landmarks.
 *
 * @param object_detection A full object detecetion as acquired from a
 *                         dlib::shape_predictor.
 * @returns a vector of dlib::chip_details to use with
 *          dlib::extract_image_chips.
 */
std::vector<dlib::chip_details> get_eyes_chip_details(
    const dlib::full_object_detection object_detection);

/**
 * Filles the displacement table to precalculate values for vector @f$d_i@f$
 * @cite timm2011.
 *
 * The function returns without modifying table_x or table_y if size is
 * smaller than the current table size.
 *
 * The size will always be set to size or size + 1, whichever is odd. This
 * ensures that the center value is 0 as a reference point.
 *
 * After a call to this function with a sufficiently big size argument,
 * table_x and table_y contain values such that reading from both tables
 * at the same indices yields the respective components for a unit vector
 * geometrically pointing from the center of the matrix to the read position.
 *
 * A @f$3 \times 3@f$ table_x would thus look like this:
 *
 * @f[
 * \left(
 * \begin{array}{ccc}
 * -0.707 & 0 & 0.707 \\
 * -1 & 0 & 1 \\
 * -0.707 & 0 & 0.707
 * \end{array}
 * \right)
 * @f]
 *
 * And the corresponding @f$3 \times 3@f$ table_y would be table_x's transposed
 * version:
 *
 * @f[
 * \left(
 * \begin{array}{ccc}
 * -0.707 & -1 & -0.707 \\
 * 0 & 0 & 0 \\
 * 0.707 & 1 & 0.707
 * \end{array}
 * \right)
 * @f]
 *
 * Accessing table_x and table_y at (0, 0) yields @f$(-0.707, -0.707)@f$ which
 * is a unit length vector pointing from the center (1, 1) to (0, 0).
 *
 * @param table_x The x components of @f$d_i@f$.
 * @param table_y The y components of @f$d_i@f$.
 * @param size The size to grow this table to.
 */
void fill_displacement_tables(dlib::matrix<double>& table_x,  // NOLINT
                              dlib::matrix<double>& table_y,  // NOLINT
                              int size);

/**
 * Normalizes a horizontal and a vertical gradient matrix to unit length vectors.
 *
 * If a relative threshold is provided, all values where the gradient magnitude
 * @f$ M @f$ is lower than @f$ \theta \max M @f$ are set to 0.
 *
 * @param horizontal A gradient, e.g. from dlib::sobel_edge_detector
 * @param vertical A gradient, e.g. from dlib::sobel_edge_detector
 * @param relative_threshold optional threshold to set small values to 0.
 */
template <typename T>
void normalize_and_threshold_gradients(
    dlib::matrix<T>& horizontal,  // NOLINT
    dlib::matrix<T>& vertical,  // NOLINT
    double relative_threshold = -1) {
  dlib::matrix<T> magnitude;
  magnitude = dlib::sqrt(dlib::squared(horizontal) +
                         dlib::squared(vertical));

  // normalization
  horizontal = dlib::pointwise_multiply(
      horizontal, dlib::reciprocal(magnitude));
  vertical = dlib::pointwise_multiply(
      vertical, dlib::reciprocal(magnitude));

  // Thresholding
  if (relative_threshold >= 0) {
    T threshold = dlib::mean(magnitude)
                + dlib::stddev(magnitude) * relative_threshold;
    for (int row = 0; row < horizontal.nr(); ++row) {
      for (int col = 0; col < horizontal.nc(); ++col) {
        if (magnitude(row, col) < threshold) {
          horizontal(row, col) = 0;
          vertical(row, col) = 0;
        }
      }
    }
  }
}

}  // namespace util

namespace pipeline {

/**
 * @class PupilLocalization pupil_localization.h "gaze/pipeline_steps/pupil_localization.h"
 * @brief Implements Timm and Barth (2011) @cite timm2011 to detect eye
 *        centers.
 *
 * This implementation uses a lookup table to pre-calculate the distance function.
 * Additionally, some tweaks are introduced, as explained in the similar
 * implementation <a href="https://github.com/trishume/eyeLike">eyeLike</a> by
 * Tristan Hume.
 *
 * For details you can also refer to Tristan's blog post
 * <a href="http://thume.ca/projects/2012/11/04/simple-accurate-eye-center-tracking-in-opencv/">Simple, accurate eye center tracking in OpenCV</a>.
 */
class PupilLocalization final
    : public PipelineStep,
      public gui::ImageVisualizeable {
  dlib::matrix<double> displacement_table_x;
  dlib::matrix<double> displacement_table_y;
  double relative_threshold_factor;
  double sigma;
  double sigma_factor;

 public:
    /**
     */
    PupilLocalization();

    /**
     * Detects the eye center for each eye.
     *
     * @param data The data object to be updated.
     * @returns via `data` the modified data object.
     */
    void process(util::Data& data) override;

    /**
     * @param data The data object to be updated.
     */
    void visualize(util::Data& data) override;
};

}  // namespace pipeline

}  // namespace gaze

#endif  // INCLUDE_GAZE_PIPELINE_STEPS_PUPIL_LOCALIZATION_H_
