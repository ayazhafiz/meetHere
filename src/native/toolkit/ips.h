#ifndef TOOLKIT_IPS_H
#define TOOLKIT_IPS_H

#include <stdint.h>

struct inner_product_space
{
  /**
   * @brief   Calculates the norm of a vector.
   *
   * @param   degree the degree of norm to calculate
   * @param   vec    vector to evaluate
   * @param   len    dimension of vector
   *
   * @return  the norm of the vector
   */
  double (*norm)(uint64_t degree, const double vec[], uint64_t dim);

  /**
   * @brief   Calculates the distance between two vectors using a norm.
   *
   * @param   degree    the degree of norm to use
   * @param   vec1      the first vector
   * @param   vec2      the second vector
   * @param   len       dimension of vectors
   *
   * @return  the distance between two vectors
   */
  double (*norm_distance)(uint64_t     degree,
                          const double vec1[],
                          const double vec2[],
                          uint64_t     dim);

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
  double (*net_distance)(uint64_t       degree,
                         const double   center[],
                         uint64_t       dim,
                         const double * neighbors[],
                         uint64_t       num_neighbors);
};

extern const struct inner_product_space IPS;

#endif
