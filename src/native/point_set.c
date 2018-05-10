#include "point_set.h"

#include "toolkit/array.h"
#include "toolkit/ips.h"

#include <stdlib.h>

//
// HELPERS
//

enum NUM_DIRS
{
  NUM_DIRS = 8
};

/*
 *           (0,1)
 *    (-S2,S2)   (S2,S2)
 * (-1,0)      x       (1,0)
 *    (-S2,-S2)  (S2,-S2)
 *          (0,-1)
 */
#define SQRT2 1.41421356237309504880
#define S2 SQRT2 / 2
static const double DELTA[][NUM_DIRS] = {{-1, -S2, 0, S2, 1, S2, 0, -S2},
                                         {0, S2, 1, S2, 0, -S2, -1, -S2}};

//
// MAIN
//

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
static double * mean(const double points[][DIM], const size_t num_points)
{
  double center[DIM] = {0};

  for (size_t i = 0; i < num_points; ++i) {
    for (size_t dim = 0; dim < DIM; ++dim) {
      center[dim] += points[i][dim];
    }
  }

  double * res = Array.New.double_array(DIM);
  for (size_t i = 0; i < DIM; ++i) {
    res[i] = center[i] / num_points;
  }

  return res;
}

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
static double * geometric_median(const double points[][DIM],
                                 const size_t num_points,
                                 const struct GeometricCenterOptions * options)
{
  // fill center to CoM, calculate initial score and step
  double * center = PointSet.mean(points, num_points);
  double   score  = IPS.net_distance(DIM,
                                  center,
                                  DIM,
                                  (const double **)points,
                                  num_points);
  double   step   = score / num_points * options->bounds;

  double * _center = Array.New.double_array(DIM);

  // descend gradient, searching for the function minimum, until the error
  // reaches some acceptable epsilon.
  while (step > options->epsilon) {
    bool improved = false;

    // check points a step in each direction to find the lowest cost
    for (size_t i = 0; i < NUM_DIRS; options->subsearch ? ++i : (i += 2)) {
      for (size_t dim = 0; dim < DIM; ++dim) {
        _center[dim] = center[dim] + step * DELTA[dim][i];
      }

      const double _score = IPS.net_distance(DIM,
                                             _center,
                                             DIM,
                                             (const double **)points,
                                             num_points);

      if (_score < score) {
        for (size_t dim = 0; dim < DIM; ++dim) {
          center[dim] = _center[dim];
        }
        score    = _score;
        improved = true;
        break;
      }
    }

    if (!improved) {  // decrease error range
      step /= 2;
    }
  }

  free(_center);

  return center;
}

//
// WRAPPERS
//

const struct point_set PointSet = {.mean             = mean,
                                   .geometric_median = geometric_median};
