#include "matrix.h"

#include "array.h"
#include "ips.h"

#include <stdlib.h>

#ifndef __D_swap
#define __D_swap(a, b, type) \
  {                          \
    type __swap = a;         \
    a           = b;         \
    b           = __swap;    \
  }
#endif

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
 * @note    This is guaranteed to reduce the matrix to row echelon form, though
 *          not necessarily reduced row echelon form.
 *
 * @param   matrix           augmented matrix
 * @param   dimension        the dimension of the submatrix and vector
 *                           comprising the augmented matrix
 */
static void eliminate_gaussian(double * matrix, const uint64_t dim)
{
  const uint64_t cols = dim + 1;
  for (uint64_t i = 0; i < dim; ++i) {
    for (uint64_t j = i + 1; j < dim; ++j) {
      // swap rows if not in descending order
      if (matrix[Array.idx_2d(j, i, cols)] > matrix[Array.idx_2d(i, i, cols)]) {
        for (uint64_t k = 0; k <= dim; ++k) {
          __D_swap(matrix[Array.idx_2d(i, k, cols)],
                   matrix[Array.idx_2d(j, k, cols)],
                   double);
        }
      }

      // eliminate lower row
      const double ratio = matrix[Array.idx_2d(j, i, cols)] /
                           matrix[Array.idx_2d(i, i, cols)];
      for (uint64_t k = 0; k <= dim; ++k) {
        matrix[Array.idx_2d(j, k, cols)] -= ratio *
                                            matrix[Array.idx_2d(i, k, cols)];
      }
    }
  }
}

/**
 * @brief   Solves a augmented matrix in reduced echelon form.
 * @details Performs backsubstitution on an augmented matrix in reduced echelon
 *          form.
 * @note    Matrix must be augmented, in reduced echelon form, and have a unique
 *          solution.
 *
 * @param   matrix           reduced augmented matrix
 * @param   dimension        the dimension of the submatrix and vector
 *                           comprising the augmented matrix
 *
 * @return  a pointer to the values of the solution vector
 */
static double * solve_reduced_augmented(const double * matrix,
                                        const uint64_t dimension)
{
  const uint64_t cols     = dimension + 1;
  double *       solution = Array.New.double_array(dimension);

  // perform backwards substitution
  for (uint64_t i = dimension; i > 0; --i) {
    const uint64_t row = i - 1;
    solution[row]      = matrix[Array.idx_2d(row, dimension, cols)];

    // subtract influence of all following coefficients
    for (uint64_t j = i; j < dimension; ++j) {
      solution[row] -= matrix[Array.idx_2d(row, j, cols)] * solution[j];
    }

    // divide by leading coefficient
    solution[i - 1] /= matrix[Array.idx_2d(i - 1, i - 1, cols)];
  }

  return solution;
}

/**
 * @brief   Creates a cost matrix based on distances among a set of vectors.
 * @details For each two vectors `i, j` in the set, the distance `|i-j|` based
 *          on some inner product space is calculated and stored in index
 *          `(i, j)` of the cost matrix.
 *
 * @param   vectors          set of vectors to build a cost matrix for
 * @param   num_vectors      number of vectors
 * @param   dimension        dimension of the vectors
 * @param   norm_degree      degree of the norm to use in calculating distance
 *                           between vectors
 *
 * @return  a pointer to the cost matrix
 */
static double * cost_matrix(const double * vectors,
                            const uint64_t num_vectors,
                            const uint64_t dimension,
                            const uint64_t norm_degree)
{
  double * cost_matrix = Array.New.double_array(num_vectors * num_vectors);

  for (uint64_t i = 0; i < num_vectors; ++i) {
    for (uint64_t j = 0; j < num_vectors; ++j) {
      double start[dimension];
      double end[dimension];
      for (uint64_t p = 0; p < dimension; ++p) {
        start[p] = vectors[Array.idx_2d(i, p, dimension)];
        end[p]   = vectors[Array.idx_2d(j, p, dimension)];
      }

      cost_matrix[Array.idx_2d(i,
                               j,
                               num_vectors)] = IPS.norm_distance(norm_degree,
                                                                 start,
                                                                 end,
                                                                 dimension);
    }
  }
  return cost_matrix;
}

/**
 * @brief   Wraps eliminate_gaussian for a better user API.
 *
 * @param   matrix           augmented matrix
 * @param   dimension        the dimension of the submatrix and vector
 *                           comprising the augmented matrix
 */
static void __WRAP_eliminate_gaussian(double *       matrix[],
                                      const uint64_t dimension)
{
  eliminate_gaussian((double *)matrix, dimension);
}

/**
 * @brief   Wraps solve_reduced_augmented for a better user API.
 *
 * @param   matrix           reduced augmented matrix
 * @param   dimension        the dimension of the submatrix and vector
 *                           comprising the augmented matrix
 *
 * @return  a pointer to the values of the solution vector
 */
static double * __WRAP_solve_reduced_augmented(const double * matrix[],
                                               const uint64_t dimension)
{
  return solve_reduced_augmented((double *)matrix, dimension);
}

/**
 * @brief   Wraps cost_matrix for a better user API.
 *
 * @param   vectors          set of vectors to build a cost matrix for
 * @param   num_vectors      number of vectors
 * @param   dimension        dimension of the vectors
 * @param   norm_degree      degree of the norm to use in calculating distance
 *                           between vectors
 *
 * @return  a pointer to the cost matrix
 */
static double * __WRAP_cost_matrix(const double * vectors[],
                                   const uint64_t num_vectors,
                                   const uint64_t dimension,
                                   const uint64_t norm_degree)
{
  return cost_matrix((double *)vectors, num_vectors, dimension, norm_degree);
}

const struct matrix Matrix =
    {.eliminate_gaussian      = __WRAP_eliminate_gaussian,
     .solve_reduced_augmented = __WRAP_solve_reduced_augmented,
     .cost_matrix             = __WRAP_cost_matrix};
