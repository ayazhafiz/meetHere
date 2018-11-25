#ifndef TSP_H
#define TSP_H

#include <stdint.h>

struct travelling_salesman_problem
{
  uint64_t * (*solve)(const double * points[],
                      uint64_t       num_points,
                      uint64_t       dimension,
                      uint64_t       start_index,
                      uint64_t       norm_degree);
};

extern const struct travelling_salesman_problem TSP;

#endif
