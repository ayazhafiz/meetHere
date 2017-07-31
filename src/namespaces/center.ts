import * as Bindings from 'bindings';

/**
 * Utilities for determining centers of sets of points
 *
 * @name Center
 * @namespace
 * @see https://stackoverflow.com/a/12934484
 * @desc heavily optimized JS version of @see
 * @todo Refactor as C++ bindings
 */
export namespace Center {
  /*
   *           (0,1)
   *    (-s2,s2)    (s2,s2)
   * (-1,0)      x       (1,0)
   *    (-s2,-s2)   (s2,-s2)
   *          (0,-1)
   */
  const s2 = Math.sqrt(2) / 2;
  const delta = {
    x: [-1, -s2, 0, s2, 1, s2, 0, -s2],
    y: [0, s2, 1, s2, 0, -s2, -1, -s2]
  };

  function cost(points: Array<Array<number>>, ...args: Array<number>);
  /**
   * Summates the Pythagorean distance between a center and an arbitrary amount
   * of points as an efficient and powerful cost function.
   *
   * @function
   * @param {Array} points 2D Array of points on a plane
   * @param {number} x x coordinate of center
   * @param {number} y y coordinate of center
   * @return {number} Sum of Pythagorean distances from center to each point
   */
  function cost(points: Array<Array<number>>, x: number, y: number): number {
    return points.reduce(
      (sum, value) =>
        sum + Math.sqrt((value[0] - x) ** 2 + (value[1] - y) ** 2),
      0
    );
  }

  /**
   * Finds the center of mass of an arbitrary amount of points using standard
   * median.
   *
   * @name Center#mass
   * @function
   * @param {Array} points 2D Array of points on a plane
   * @return {object} Center of mass
   */
  export function mass(
    points: Array<Array<number>>
  ): { center: Array<number>; score: number } {
    const center = [
      points.reduce((sum, value) => sum + value[0], 0) / points.length,
      points.reduce((sum, value) => sum + value[1], 0) / points.length
    ];
    return { center: center, score: cost(points, ...center) };
  }

  export function geometric(
    points: Array<Array<number>>,
    ...args: Array<number | boolean>
  );
  /**
   * Calculates the geometric center of an arbitrary amount of points.
   *
   * This is done through a simple Newtonian search; i.e. we iterate an
   * indiscriminate amount of times through smaller bounds until we approve some
   * margin of error (epsilon). Note that local maxima is a non-issue, as the
   * geometric median is (unique and covergent for non-co-linear points)[http://www.stat.rutgers.edu/home/cunhui/papers/39.pdf].
   *
   * @name Center#geometric
   * @function
   * @param {Array} points 2D Array of points on a plane
   * @param {boolean} [subsearch=false] Whether to search for efficiencies obliquely
   * @param {number} [epsilon=1e-3] Precision of geometric center
   * @param {number} [bounds=10] Starting unit bounds for center calculation
   * @return {object} Geometric center
   */
  export function geometric(
    points: Array<Array<number>>,
    subsearch: boolean,
    epsilon: number,
    bounds: number
  ): { center: Array<number>; score: number } {
    return Bindings('center').geometric(points, subsearch, epsilon, bounds);
  }
}
