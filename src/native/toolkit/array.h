#ifndef TOOLKIT_ARRAY_H
#define TOOLKIT_ARRAY_H

#include <stddef.h>

struct new_
{
  /**
   * @brief   Creates a dynamic array of doubles initialized to zero.
   *
   * @param   len               length of the array
   *
   * @return  pointer to the array
   */
  double * (*double_array)(size_t len);

  /**
   * @brief   Creates a dynamic array of size_t initialized to zero.
   *
   * @param   len               length of the array
   *
   * @return  pointer to the array
   */
  size_t * (*size_t_array)(size_t len);
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
  size_t (*idx_2d)(size_t row, size_t col, size_t num_cols);
};

extern const struct array Array;

#endif
