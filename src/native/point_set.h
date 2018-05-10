#ifndef POINT_SET_H
#define POINT_SET_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>

enum DIM
{
  DIM = 2
};

/**
 * @struct
 * @brief  Options for how the geometric center should be calculated
 *
 * @prop   epsilon   acceptable margin of error
 * @prop   bounds    a multiplier of the range of points to search
 * @prop   subsearch whether to search obliquely
 */
struct GeometricCenterOptions
{
  const double epsilon;
  const double bounds;
  const bool   subsearch;
};

struct point_set
{
  /**
   * @brief   Finds the mean of a set of 2D points.
   * @details Assumes all points have equal weight. Puts the center of mass in a
   *          user-designated array.
   *
   * @param   points     points to measure
   * @param   num_points number of points
   *
   * @return  pointer to mean of points
   */
  double * (*mean)(const double points[][DIM], size_t num_points);

  /**
   * @brief   Finds the geometric median of a set of 2D points.
   * @details Fills an array with the geometric center of an arbitrary amount of
   *          points. Returns the score (total cost to center) of the geometric
   *          center.
   *          The algorithm is a simple Newtonian search. We iterate an
   *          indiscriminate amount of times through smaller bounds until we
   *          approve some margin of error. Note that local maxima are a
   *          non-issue, as the geometric median is (unique and covergent for
   *          non-co-linear
   *          points)[http://www.stat.rutgers.edu/home/cunhui/papers/39.pdf].
   *
   * @param   points     points to find the center of
   * @param   num_points number of points
   * @param   options    specified margin of error, bound range, and subsearch
   *                     value
   *
   * @return  pointer to geometric median of points
   */
  double * (*geometric_median)(const double points[][DIM],
                               size_t       num_points,
                               const struct GeometricCenterOptions * options);
};

extern const struct point_set PointSet;

#endif
