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
    // Initially, our center is the median and our score is that of the median.
    const com = mass(points);
    let [center, score] = [com.center, com.score];

    // Assign a step `bounds` times the average center-to-point distance for
    // improved coverage.
    let step = score / points.length * bounds;

    while (step > epsilon) {
      let improved = false;
      for (let i = 0; i < delta.x.length; subsearch ? ++i : (i += 2)) {
        const _nx = center[0] + step * delta.x[i];
        const _ny = center[1] + step * delta.y[i];
        const _nScore = cost(points, _nx, _ny);

        // If the algorithm has improved our score, we apply it...
        if (_nScore < score) {
          [center, score] = [[_nx, _ny], _nScore];
          improved = true;
          break;
        }
      }

      // ...otherwise, we decrease our step.
      if (!improved) {
        step /= 2;
      }
    }
    return { center: center, score: score };
  }

  export function anneal(points: Array<Array<number>>) {
    const com = mass(points);
    let [center, score] = [com.center, com.score];

    let temp = 1.0;
    const min = 1e-5;
    const alpha = 0.9;

    while (temp > min) {
      temp *= alpha;
    }

    return { center: center, score: score };
  }
}
