#include "array.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief   Creates a dynamic array of doubles initialized to zero.
 *
 * @param   len               length of the array
 *
 * @return  pointer to the array
 */
static inline double * double_array(const uint64_t len)
{
  double * res = (double *)malloc(len * sizeof(double));
  memset(res, 0, len * sizeof(double));
  return res;
}

/**
 * @brief   Creates a dynamic array of uint64_t initialized to zero.
 *
 * @param   len               length of the array
 *
 * @return  pointer to the array
 */
static inline uint64_t * uint64_t_array(const uint64_t len)
{
  uint64_t * res = (uint64_t *)malloc(len * sizeof(uint64_t));
  memset(res, 0, len * sizeof(uint64_t));
  return res;
}

/**
 * @brief   Converts an index in a 2D array to the corresponding index in a
 *          flattened array.
 *
 * @param   row              the row index of the 2D array
 * @param   col              the column index of the 2D array
 * @param   num_cols         the number of columns in the 2D array
 *
 * @return  the corresponding index in the flattened array
 */
static inline uint64_t idx_2d(const uint64_t row,
                              const uint64_t col,
                              const uint64_t num_cols)
{
  return row * num_cols + col;
}

const struct array Array = {.New    = {.double_array   = double_array,
                                    .uint64_t_array = uint64_t_array},
                            .idx_2d = idx_2d};
