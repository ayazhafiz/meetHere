#include "array.h"

#include <stdlib.h>
#include <string.h>

//
// HELPERS
//

//
// MAIN
//

/**
 * @brief   Creates a dynamic array of doubles initialized to zero.
 *
 * @param   len               length of the array
 *
 * @return  pointer to the array
 */
static inline double * double_array(const size_t len)
{
  double * res = (double *)malloc(len * sizeof(double));
  memset(res, 0, len * sizeof(double));
  return res;
}

/**
 * @brief   Creates a dynamic array of size_t initialized to zero.
 *
 * @param   len               length of the array
 *
 * @return  pointer to the array
 */
static inline size_t * size_t_array(const size_t len)
{
  size_t * res = (size_t *)malloc(len * sizeof(size_t));
  memset(res, 0, len * sizeof(size_t));
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
static inline size_t idx_2d(const size_t row,
                            const size_t col,
                            const size_t num_cols)
{
  return row * num_cols + col;
}

//
// WRAPPERS
//

const struct array Array = {.New    = {.double_array = double_array,
                                    .size_t_array = size_t_array},
                            .idx_2d = idx_2d};
