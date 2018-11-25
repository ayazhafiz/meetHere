#include "point_set.h"

#include "toolkit/array.h"
#include "toolkit/ips.h"

#include <stdlib.h>

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
static const struct
{
  double x[NUM_DIRS];
  double y[NUM_DIRS];
} DELTA = {{-1, -S2, 0, S2, 1, S2, 0, -S2}, {0, S2, 1, S2, 0, -S2, -1, -S2}};

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
static Grid_2D mean(const double points[][DIM2], const uint64_t num_points)
{
  Grid_2D center = {0, 0};
  for (uint64_t i = 0; i < num_points; ++i) {
    center.x += points[i][0];
    center.y += points[i][1];
  }

  center.x = center.x / num_points;
  center.y = center.y / num_points;

  return center;
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
static Grid_2D geometric_median(const double   points[][DIM2],
                                const uint64_t num_points,
                                const struct GeometricCenterOptions * options)
{
  // fill center to CoM, calculate initial score and step
  Grid_2D center             = PointSet.mean(points, num_points);
  double  __center_arr[DIM2] = {center.x, center.y};
  double  score              = IPS.net_distance(DIM2,
                                  (const double *)__center_arr,
                                  DIM2,
                                  (const double **)points,
                                  num_points);
  double  step               = score / num_points * options->bounds;

  // descend gradient, searching for the function minimum, until the error
  // reaches some acceptable epsilon.
  while (step > options->epsilon) {
    bool improved = false;

    // check points a step in each direction to find the lowest cost
    for (uint64_t i = 0; i < NUM_DIRS; options->subsearch ? ++i : (i += 2)) {
      __center_arr[0] = center.x + step * DELTA.x[i];
      __center_arr[1] = center.y + step * DELTA.y[i];

      const double _score = IPS.net_distance(DIM2,
                                             (const double *)__center_arr,
                                             DIM2,
                                             (const double **)points,
                                             num_points);

      if (_score < score) {
        center.x = __center_arr[0];
        center.y = __center_arr[1];
        score    = _score;
        improved = true;
        break;
      }
    }

    if (!improved) {  // decrease error range
      step /= 2;
    }
  }

  return center;
}

const struct point_set PointSet = {.mean             = mean,
                                   .geometric_median = geometric_median};
