#ifndef TSP_H
#define TSP_H

#include <stddef.h>

struct travelling_salesman_problem
{
  size_t * (*solve)(const double * points[],
                    size_t         num_points,
                    size_t         dimension,
                    size_t         start_index,
                    size_t         norm_degree);
};

extern const struct travelling_salesman_problem TSP;

#endif
