#include "polynomial.h"

#include "toolkit/array.h"
#include "toolkit/matrix.h"

#include <math.h>
#include <stdlib.h>

//
// HELPERS
//

#ifndef __D_swap
#define __D_swap(a, b, type) \
  {                          \
    type __swap = a;         \
    a           = b;         \
    b           = __swap;    \
  }
#endif

static const size_t DEFAULT_DEGREE = 2;

/**
 * @brief   Generates an augmented Vandermonde matrix for a polynomial of
 *          dimension `k`, consisting of a `(k + 1) x (k + 1)` Vandermonde
 *          matrix and a projected vector of dimension `k + 1`.
 * @details Expands the unique values of the Vandermonde matrix into a
 *          submatrix V, and places the projected vector in a subvector p on the
 *          RHS. Visually, the augmented matrix A becomes
 *            A = | V , p |
 *              = | n           ...  Σ^n(x_i^k)      | Σ^n(y_i)         |
 *                | Σ^n(x_i)    ...  Σ^n(x_i^(k+1))  | Σ^n(x_i * y_i)   |
 *                | ...         ...  ...             | ...              |
 *                | Σ^n(x_i^k)  ...  Σ^n(x_i^(2k))   | Σ^n(x_i^k * y_i) |.
 * @note    Returns a pointer to the flattened matrix.
 *
 * @param   V_unique          unique values of the Vandermonde matrix
 * @param   p                 the projected vector
 * @param   k                 dimension of the polynomial described by the
 *                            system
 *
 * @return  a pointer to the values of the augmented Vandermonde matrix.
 */
static double * __augmented_vandermonde(const double * V_uniq,
                                        const double * p,
                                        const size_t   k)
{
  const size_t rows = k + 1;
  const size_t cols = k + 2;
  double *     A    = Array.New.double_array(rows * cols);

  for (size_t r = 0; r < (k + 1); ++r) {
    for (size_t c = 0; c < (k + 1); ++c) {
      A[r * cols + c] = V_uniq[r + c];  // store values of Vandermonde matrix
    }

    size_t last_in_row = r * cols + k + 1;  // [r][cols-1]; i.e. [r][k + 1]
    A[last_in_row]     = p[r];              // store projected vector
  }

  return A;
}

/**
 * @brief   Generates the unique values of a Vandermonde matrix.
 * @details Given a set of `n` points of the form `(x,y)`, the Vandermonde
 *          matrix describing the best-fit polynomial of dimension `k` for
 *          that set has the unique values
 *            n, Σ^n(x_i), ... , Σ^n(x_i^k), ... , Σ^n(x_i^(2k)).
 *
 * @param   x_points          set of x point coordinates
 * @param   num_points        number of points
 * @param   k                 dimension of polynomial to generate a
 *                            Vandermonde matrix for
 *
 * @return  a pointer to an array of the unique values of the best-fit
 *          polynomial's Vandermonde matrix.
 */
static double * __vandermonde(const double x_points[],
                              const size_t num_points,
                              const size_t k)
{
  const size_t len    = 2 * k + 1;
  double *     vmonde = Array.New.double_array(len);

  for (size_t deg = 0; deg < len; ++deg) {
    for (size_t i = 0; i < num_points; ++i) {  // Σ^n(x_i^(`deg`))
      vmonde[deg] += pow(x_points[i], deg);
    }
  }
  return vmonde;
}

/**
 * @brief   Creates a projected vector.
 * @details Given a set of `n` points of the form `(x,y)`, the projected
 *          vector `b` describing the best-fit polynomial of dimension `k` for
 *          that set is of the form
 *            | Σ^n(y_i)         |
 *            | Σ^n(x_i * y_i)   |
 *            | ...              |
 *            | Σ^n(x_i^k * y_i) |.
 *
 * @param   x_points          set of x point coordinates
 * @param   y_points          set of y point coordinates
 * @param   num_points        number of points
 * @param   k                 dimension of polynomial to generate a projected
 *                            vector for
 *
 * @return  a pointer to an array of the values of the projected vector.
 */
static double * __projected_vector(const double x_points[],
                                   const double y_points[],
                                   const size_t num_points,
                                   const size_t k)
{
  const size_t len = k + 1;
  double *     vec = Array.New.double_array(len);

  for (size_t deg = 0; deg < len; ++deg) {
    for (size_t i = 0; i < num_points; ++i) {  // Σ^n(x_i^(`deg`) * y_i)
      vec[deg] += pow(x_points[i], deg) * y_points[i];
    }
  }
  return vec;
}

//
// MAIN
//

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
static size_t guess_degree(const double x[],
                           const double y[],
                           const size_t num_points)
{
  if (num_points <= 1) {  // 0-degree function
    return 0;
  }

  // copy, then sort y coordinates
  double * sorted_y = Array.New.double_array(num_points);
  for (size_t i = 0; i < num_points; ++i) {
    sorted_y[i] = y[i];
  }
  {  // bubble sort
    int    sorted = 0;
    size_t pass   = 0;

    while (sorted != 1) {
      sorted = 1;
      ++pass;
      for (size_t i = 0; i < num_points - pass; ++i) {
        if (x[i] > x[i + 1]) {
          __D_swap(sorted_y[i], sorted_y[i + 1], double);
          sorted = 0;
        }
      }
    }
  }

  // count extrema
  int    dir     = sorted_y[0] < sorted_y[1] ? 1 : -1;
  size_t extrema = 0;
  for (size_t i = 1; i < num_points - 1; ++i) {
    const int _dir = sorted_y[i] < sorted_y[i + 1] ? 1 : -1;
    if (dir != _dir) {  // change of slope sign
      ++extrema;
      dir = _dir;
    }
  }

  free(sorted_y);

  return DEFAULT_DEGREE + extrema;
}

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
static double * best_fit(const double x_points[],
                         const double y_points[],
                         const size_t num_points,
                         const size_t polynomial_degree)
{
  const size_t dimension = polynomial_degree + 1;

  double * M           = __vandermonde(x_points, num_points, polynomial_degree);
  double * b           = __projected_vector(x_points,
                                  y_points,
                                  num_points,
                                  polynomial_degree);
  double * augmented_M = __augmented_vandermonde(M, b, polynomial_degree);

  Matrix.eliminate_gaussian((double **)augmented_M, dimension);

  double * x = Matrix.solve_reduced_augmented((const double **)augmented_M,
                                              dimension);

  free(M);
  free(b);
  free(augmented_M);

  return x;
}

//
// WRAPPERS
//

const struct polynomial Polynomial = {.guess_degree = guess_degree,
                                      .best_fit     = best_fit};
