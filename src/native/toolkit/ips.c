#include "ips.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//
// HELPERS
//

//
// MAIN
//

/**
 * @brief  Calculates the norm of a vector.
 *
 * @param  degree the degree of norm to calculate
 * @param  vec    vector to evaluate
 * @param  len    dimension of vector
 *
 * @return the norm of the vector
 */
static double norm(const size_t degree, const double vec[], const size_t dim)
{
  double sum = 0;
  for (size_t i = 0; i < dim; ++i) {
    sum += pow(fabs(vec[i]), degree);
  }
  return pow(sum, (1.0 / degree));
}

/**
 * @brief   Calculates the distance between two vectors using a norm.
 *
 * @param   degree    the degree of norm to use
 * @param   vec1      the first vector
 * @param   vec2      the second vector
 * @param   len        dimension of vectors
 *
 * @return  the distance between two vectors
 */
static double norm_distance(const size_t degree,
                            const double vec1[],
                            const double vec2[],
                            const size_t dim)
{
  double sum = 0;
  for (size_t i = 0; i < dim; ++i) {
    sum += pow(fabs(vec1[i] - vec2[i]), degree);
  }
  return pow(sum, (1.0 / degree));
}

/**
 * @brief   Calculates the net distance between a central point and a set of
 *          neighbors.
 * @details Summates the norms of all vectors defined from the central point
 *          to each neighboring point.
 *
 * @param   degree        the degree of norm to calculate
 * @param   center        the central point
 * @param   dim           dimension of the vectors
 * @param   neighbors     neighbors to evaluate
 * @param   num_neighbors number of neighbors
 *
 * @return  the net distance of vectors from the center to each neighbor
 */
static double net_distance(const size_t degree,
                           const double center[],
                           const size_t dim,
                           const double neighbors[],
                           const size_t num_neighbors)
{
  double sum = 0;
  for (size_t i = 0; i < num_neighbors; ++i) {
    sum += IPS.norm_distance(degree, center, (neighbors + i * dim), dim);
  }
  return sum;
}

//
// WRAPPERS
//

/**
 * @brief   Wraps net_distance for a better user API.
 *
 * @param   degree        the degree of norm to calculate
 * @param   center        the central point
 * @param   dim           dimension of the vectors
 * @param   neighbors     neighbors to evaluate
 * @param   num_neighbors number of neighbors
 *
 * @return  the net norm of vectors from the center to each neighbor
 */
static double __WRAP_net_distance(const size_t   degree,
                                  const double   center[],
                                  const size_t   dim,
                                  const double * neighbors[],
                                  const size_t   num_neighbors)
{
  return net_distance(degree,
                      center,
                      dim,
                      (const double *)neighbors,
                      num_neighbors);
}

const struct inner_product_space IPS = {.norm          = norm,
                                        .norm_distance = norm_distance,
                                        .net_distance  = __WRAP_net_distance};
