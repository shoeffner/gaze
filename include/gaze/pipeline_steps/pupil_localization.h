// Copyright 2017 Sebastian Höffner

#ifndef INCLUDE_GAZE_PIPELINE_STEPS_PUPIL_LOCALIZATION_H_
#define INCLUDE_GAZE_PIPELINE_STEPS_PUPIL_LOCALIZATION_H_

#include "gaze/pipeline_step.h"
#include "gaze/util/data.h"


namespace gaze {

namespace pipeline {

/**
 * @class PupilLocalization pupil_localization.h "gaze/pipeline_steps/pupil_localization.h"
 * @brief Implements Timm and Barth (2011) @cite timm2011 to detect eye
 *        centers.
 *
 * // TODO(shoeffner): Write documentation for PupilLocalization.
 */
class PupilLocalization final : public PipelineStep {
  dlib::matrix<double> displacement_table_x;
  dlib::matrix<double> displacement_table_y;

  // TODO(shoeffner): Add unit tests.

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
  void fill_displacement_tables(dlib::matrix<double>& table_x,
                                dlib::matrix<double>& table_y,
                                int size);

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
