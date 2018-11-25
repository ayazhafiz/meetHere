#include "tsp.h"

#include "toolkit/array.h"
#include "toolkit/matrix.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief   Finds the nearest, unvisited point to a specified one.
 * @details Iterates over a cost matrix to find the point `k` nearest a
 *          specified point `o`, with the constraint that `k` has not already
 *          been visited.
 *
 * @param   cost_matrix       distances among a set of points, where the indeces
 *                            correspond to the points compared
 * @param   o                 index of the point to find the nearest point `k`
 *                            from
 * @param   num_points        number of points
 * @param   visited_points    array specifying if each point has been visited
 *
 * @return  index of nearest point, or -1 if there are no points left
 */
static int __nearest_unvisited_point(const double * cost_matrix,
                                     const uint64_t o,
                                     const uint64_t num_points,
                                     const uint64_t visited_points[])
{
  int      k        = -1;
  uint64_t min_cost = SIZE_MAX;

  for (uint64_t cand = 0; cand < num_points; ++cand) {
    const uint64_t _min_cost = cost_matrix[Array.idx_2d(o, cand, num_points)];

    if (visited_points[cand] == 0) {
      if (_min_cost < min_cost) {
        min_cost = _min_cost;
        k        = cand;
      }
    }
  }

  return k;
}

/**
 * @brief   Solves the travelling salesman problem for a set of points.
 * @details Creates a cost matrix for travelling between points, then travels
 *          to the consequently nearest points until all points have been
 *          traversed.
 *
 * @param   points           set of points to solve the TSP for
 * @param   num_points       number of points
 * @param   dimension        dimension of the point vectors
 * @param   start_index      point from which to find a solution to
 * @param   norm_degree      degree of the norm to use in calculating distance
 *                           between point vectors
 *
 * @return  a pointer to the indeces to travel, in order
 */
static uint64_t * solve(const double * points,
                        const uint64_t num_points,
                        const uint64_t dimension,
                        const uint64_t start_index,
                        const uint64_t norm_degree)
{
  double *   cost_matrix    = Matrix.cost_matrix((const double **)points,
                                            num_points,
                                            dimension,
                                            norm_degree);
  uint64_t * visited_points = Array.New.uint64_t_array(num_points);
  uint64_t * travel_order   = Array.New.uint64_t_array(num_points);
  travel_order[0]           = start_index;

  uint64_t current_point = start_index;
  uint64_t idx           = 1;

  while (idx < num_points) {
    visited_points[current_point] = 1;
    const int nearest_point       = __nearest_unvisited_point(cost_matrix,
                                                        current_point,
                                                        num_points,
                                                        visited_points);

    travel_order[idx] = (uint64_t)nearest_point;
    current_point     = (uint64_t)nearest_point;
    ++idx;
  }

  free(cost_matrix);
  free(visited_points);

  return travel_order;
}

/**
 * @brief   Wraps solve for a better user API.
 *
 * @param   points           set of points to solve the TSP for
 * @param   num_points       number of points
 * @param   dimension        dimension of the point vectors
 * @param   start_index      point from which to find a solution to
 * @param   norm_degree      degree of the norm to use in calculating distance
 *                           between point vectors
 *
 * @return  a pointer to the indeces to travel, in order
 */
static uint64_t * __WRAPPER_solve(const double * points[],
                                  uint64_t       num_points,
                                  uint64_t       dimension,
                                  uint64_t       start_index,
                                  uint64_t       norm_degree)
{
  return solve((const double *)points,
               num_points,
               dimension,
               start_index,
               norm_degree);
}

const struct travelling_salesman_problem TSP = {.solve = __WRAPPER_solve};
