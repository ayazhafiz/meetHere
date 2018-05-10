#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stddef.h>

struct polynomial
{
  /**
   * @brief   Guesses the optimal degree of a polynomial function best fitting a
   *          set of points.
   * @details Sorts the points in increasing function order, and then records
   *          the number of extrema observed while traversing the points.
   *
   * @param   x                 x coordinates of the points
   * @param   y                 y coordinates of the points
   * @param   num_points        number of points
   *
   * @return  best degree of polynomial to approximate
   */
  size_t (*guess_degree)(const double x[], const double y[], size_t num_points);

  /**
   * @brief   Calculates the best-fit polynomial function for a set of 2D
   *          points.
   * @details Determines the least squares solution for a projection of a
   *          solution set onto a polynomial vector space of dimension
   *          `k = polynomial_degree`, generalized by
   *            y = a_0 + a_1(x) + ... + a_k(x^k).
   *          For a set of `n = num_points` points, a Vandermonde matrix `M` and
   *          projected vector `b` are created to solve for the solution vector
   *          `x`, Mx = b where
   *            M = | n           Σ^n(x_i)    ...  Σ^n(x_i^k)      |
   *                | Σ^n(x_i)    Σ^n(x_i^2)  ...  Σ^n(x_i^(k+1))  |
   *                | ...         ...         ...  ...             |
   *                | Σ^n(x_i^k)  ...         ...  Σ^n(x_i^(2k))   |
   *
   *            b = | Σ^n(y_i)         |     x = | a_0 |
   *                | Σ^n(x_i * y_i)   |         | a_1 |
   *                | ...              |         | ... |
   *                | Σ^n(x_i^k * y_i) |  ,      | a_k |  .
   *
   *          Note that the only unique values in `M` are `n, Σ^n(x_i), ... ,
   *          Σ^n(x_i^k), ... , Σ^n(x_i^(2k))`. An optimization can be made by
   *          only evaluating these `2k + 1` values.
   *
   * @param   x_points          set of x point coordinates
   * @param   y_points          set of y point coordinates
   * @param   num_points        number of points
   * @param   polynomial_degree  degree of polynomial function to approximate
   *
   * @return  a pointer to a matrix of length `polynomial_degree + 1` with the
   *          values of the solution vector; i.e. with the coefficients of the
   *          best-fit polynomial of degree `polynomial_degree`.
   */
  double * (*best_fit)(const double x_points[],
                       const double y_points[],
                       size_t       num_points,
                       size_t       polynomial_degree);
};

extern const struct polynomial Polynomial;

#endif
