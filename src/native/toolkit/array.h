#ifndef TOOLKIT_ARRAY_H
#define TOOLKIT_ARRAY_H

#include <stdint.h>

struct new_
{
  /**
   * @brief   Creates a dynamic array of doubles initialized to zero.
   *
   * @param   len               length of the array
   *
   * @return  pointer to the array
   */
  double * (*double_array)(uint64_t len);

  /**
   * @brief   Creates a dynamic array of uint64_t initialized to zero.
   *
   * @param   len               length of the array
   *
   * @return  pointer to the array
   */
  uint64_t * (*uint64_t_array)(uint64_t len);
};

struct array
{
  const struct new_ New;

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
  uint64_t (*idx_2d)(uint64_t row, uint64_t col, uint64_t num_cols);
};

extern const struct array Array;

#endif
