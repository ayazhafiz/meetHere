import { CenterOptions } from './interfaces/index';
import { arrayUtil as importArrayUtil } from './util/array';
import * as Bindings from 'bindings';
const CLIB = Bindings('api');
const Method = {
  tsp: 't'.charCodeAt(0),
  naiveVrp: 'n'.charCodeAt(0),
};

importArrayUtil();

/**
 * A prototype describing a set of points on a plane.
 *
 * ```
 * import { Position } from 'meethere';
 *
 * const options = {
 *   subsearch: true,
 *   epsilon: 1e-4,
 *   bounds: 15
 * }
 *
 * let Plane = new Position(
 *   [ [5, 6.1], [-2.07, -1.33], [9.8, -0.02] ],
 *   options
 * );
 * Plane.add([1, -4]);
 * Plane.center // => [ 2.2460648128951566, -0.853617418121833 ]
 * Plane.score // => 0.010113270070291593
 * ```
 *
 * @class
 */
class Position {
  locations: Array<Array<number>>;
  options: CenterOptions;

  /**
   * Default geometric center options
   *
   * @constant
   * @type {CenterOptions}
   * @default
   */
  static defaultCenterOptions: CenterOptions = {
    subsearch: false,
    epsilon: 1e-3,
    bounds: 10,
    startIndex: 0,
    degree: null,
  };

  /**
   * Creates a Position on a plane described by a set of locations.
   *
   * @constructs
   * @param {Array} locations 2D Array of points on a plane
   * @param {CenterOptions} [options=Position.defaultCenterOptions] General
   * search options
   */
  constructor(locations: Array<Array<number>>, options: CenterOptions = {}) {
    this.locations = locations;
    this.options = { ...Position.defaultCenterOptions, ...options };
  }

  /**
   * Adds a location to the set of points.
   *
   * @name Position#add
   * @function
   * @param {Array} location Point to add
   *
   * ```
   * let plane = new Position([[0, 1]]);
   * plane.add([1, 0]); // => [[0, 1], [1, 0]]
   * ```
   */
  add(location: Array<number>): void {
    this.locations.push(location);
  }

  /**
   * Removes a location to the set of points.
   *
   * @name Position#remove
   * @function
   * @param {Array} location Point to remove
   * @return {Array|number} The removed location, or `-1` if no match is found
   *
   * ```
   * let plane = new Position([[2, 3], [5, 6]]);
   * plane.remove([2, 3]); // => [[5, 6]]
   * ```
   */
  remove(location: Array<number>): Array<number> | number {
    const idx = this.locations.deepIndexOf(location);
    if (idx > -1) {
      return this.locations.splice(idx, 1)[0];
    }
    return idx;
  }

  /**
   * Moves (replaces) an existing location.
   *
   * @name Position#move
   * @function
   * @param {Array} location Point to move
   * @param {Array} to Value to move to
   * @return {Array|number} The previous location, or `-1` if no match is found
   *
   * ```
   * let plane = new Position([[0, 1]]);
   * plane.move([0, 1], [9, 10]); // => [[9, 10]]
   * ```
   */
  move(location: Array<number>, to: Array<number>): Array<number> | number {
    const idx = this.locations.deepIndexOf(location);
    if (idx > -1) {
      return this.locations.splice(idx, 1, to)[0];
    }
    return idx;
  }

  /**
   * Calculates the geometric center of the Position.
   *
   * @name Position#center
   * @see https://stackoverflow.com/a/12934484
   * @desc heavily optimized, natively binded version of @see
   * @function
   * @return {Array} Geometric center of the Position
   *
   * ```
   * let plane = new Position([[0, 0], [0, 1], [1, 0]]);
   * plane.center; // => [0.21198, 0.21198]
   * ```
   */
  get center(): Array<number> {
    return CLIB.geometric(
      this.locations,
      this.options.subsearch,
      this.options.epsilon,
      this.options.bounds,
    ).center;
  }

  /**
   * Calculates the mean of the Position.
   *
   * @name Position#mean
   * @desc a rudimentary estimate for Position#center
   * @function
   * @return {Array} Geometric center of the Position
   *
   * ```
   * let plane = new Position([[0, 0], [0, 1], [1, 0]]);
   * plane.center; // => [0.33333, 0.33333]
   * ```
   */
  get mean(): Array<number> {
    return CLIB.mean(this.locations).center;
  }

  /**
   * Returns the index order of the least-costly path between all locations on
   * the plane through a solution of the TSP (~80 point efficiency).
   *
   * @name Position#bestPath
   * @TODO More involved TSP solution (figure out or-tools bindings)
   * @function
   * @return {Array} Order of indeces of the locations on the plane that gives
   * the shortest path
   *
   * ```
   * let plane = new Position([[0, 0], [5, 10], [3, 4]]);
   * plane.bestPath; // => [0, 2, 1]
   * ```
   */
  get bestPath(): Array<number> {
    return CLIB.tsp(this.locations, this.options.startIndex, Method['tsp']);
  }

  /**
   * Returns the index order of the least-costly manhattan-style drive between
   * all locations on the plane through a solution of the VRP (~80 point
   * efficiency).
   *
   * @name Position#quickPath
   * @TODO More involved VRP solution (figure out or-tools bindings)
   * @function
   * @return {Array} Order of indeces of the locations on the plane that gives
   * the shortest manhattan path
   *
   * ```
   * let plane = new Position([[0, 0], [5, 10], [3, 4]]);
   * plane.quickPath; // => [0, 2, 1]
   * ```
   */
  get quickPath() {
    return CLIB.tsp(
      this.locations,
      this.options.startIndex,
      Method['naiveVrp'],
    );
  }

  /**
   * Returns the coefficients of a n-degree polynomial best-fit to the locations
   * on the plane. Degree is specified during class instantiation, and is auto-
   * calculated by default.
   *
   * @name Position#polynomial
   * @function
   * @return {Array} an n-length array of the coefficients of a best-fit,
   * n-degree polynomial, where each index corresponds to its degree.
   *
   * ```
   * let plane = new Position([[0, 1], [1, 7], [2, 21]]);
   * plane.polynomial; // => [1, 4, 9] { 1(x^0) + 4(x^1) + 9(x^2) }
   * ```
   */
  get polynomial(): Array<number> {
    return CLIB.bestFit(this.locations, this.options.degree);
  }

  /**
   * Calculates the net cost of travelling from the points to their mean.
   *
   * @name Position#meanCost
   * @function
   * @return {number} Cost of travelling
   *
   * ```
   * let plane = new Position([[0, 1], [1, 14], [2, 45]]);
   * plane.meanCost; // => 50.04629
   * ```
   */
  get meanCost(): number {
    return CLIB.mean(this.locations).score;
  }

  /**
   * Calculates the net cost of travelling from the points to their geometric
   * center.
   *
   * @name Position#centerCost
   * @function
   * @return {number} Cost of travelling
   *
   * ```
   * let plane = new Position([[0, 1], [1, 14], [2, 45]]);
   * plane.centerCost; // => 44.05482
   * ```
   */
  get centerCost(): number {
    return CLIB.geometric(
      this.locations,
      this.options.subsearch,
      this.options.epsilon,
      this.options.bounds,
    ).score;
  }

  /**
   * Calculates the percent improvement of Position#center as compared to
   * Position#mean in each dimension.
   *
   * @name Position#geometricSignificance
   * @function
   * @return {Array} Median of the Position
   *
   * ```
   * let plane = new Position([[0, 1], [1, 14], [2, 45]]);
   * plane.geometricSignificance; // => 0.11971858482905647
   * ```
   */
  get geometricSignificance(): number {
    const [mean, center] = [this.meanCost, this.centerCost];
    return (mean - center) / mean;
  }
}

export { Position, CLIB };
