#ifndef TOOLKIT_MATRIX_H
#define TOOLKIT_MATRIX_H

#include <stddef.h>

struct matrix
{
  /**
   * @brief   Performs in-place gaussian elimination on an augmented matrix.
   * @details Traverses the main diagonal in an `n x (n + 1)` augmented matrix,
   *          with primary matrix of size `n x n`. At each point `(i, i)` in the
   *          main diagonal, the coefficient `c` at `(i, i)` is compared to the
   *          coefficient `d` at column `i` of all lower rows. If `d > c`, the
   *          rows are swapped, bubbling zero rows to the bottom of the matrix.
   *          The lower rows are then eliminated using the third elementary row
   *          operation, producing a leading variable in row `i`.
   * @note    The matrix must be augmented and consistent.
   * @note    This is guaranteed to reduce the matrix to row echelon form,
   *          though not necessarily reduced row echelon form.
   *
   * @param   matrix           augmented matrix
   * @param   dimension        the dimension of the submatrix and vector
   *                           comprising the augmented matrix
   */
  void (*eliminate_gaussian)(double * matrix[], size_t dimension);

  /**
   * @brief   Solves a augmented matrix in reduced echelon form.
   * @details Performs backsubstitution on an augmented matrix in reduced
   *          echelon form.
   * @note    Matrix must be augmented, in reduced echelon form, and have a
   *          unique solution.
   *
   * @param   matrix           reduced augmented matrix
   * @param   dimension        the dimension of the submatrix and vector
   *                           comprising the augmented matrix
   *
   * @return  a pointer to the values of the solution vector
   */
  double * (*solve_reduced_augmented)(const double * matrix[],
                                      size_t         dimension);

  /**
   * @brief   Creates a cost matrix based on distances among a set of vectors.
   * @details For each two vectors `i, j` in the set, the distance `|i-j|`
   *          based on some inner product space is calculated and
   *          stored in index `(i, j)` of the cost matrix.
   *
   * @param   vectors          set of vectors to build a cost matrix for
   * @param   num_vectors      number of vectors
   * @param   dimension        dimension of the vectors
   * @param   norm_degree      degree of the norm to use in calculating distance
   *                           between vectors
   *
   * @return  a pointer to the cost matrix
   */
  double * (*cost_matrix)(const double * vectors[],
                          size_t         num_vectors,
                          size_t         dimension,
                          size_t         norm_degree);
};

extern const struct matrix Matrix;

#endif
