import { Center } from './namespaces/center';
import { CenterOptions } from './interfaces/index';

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
export class Position {
  locations: Array<Array<number>>;
  options: CenterOptions;

  /**
   * Default geometric center options
   *
   * @constant
   * @type {SearchOptions}
   * @default
   */
  static defaultCenterOptions: CenterOptions = {
    subsearch: false,
    epsilon: 1e-3,
    bounds: 10
  };

  /**
   * Creates a Position on a plane described by a set of locations.
   *
   * @constructs
   * @param {Array} locations 2D Array of points on a plane
   * @param {CenterOptions} [options=Position.defaultCenterOptions] Geometric center search options
   */
  constructor(
    locations: Array<Array<number>>,
    options: CenterOptions = Position.defaultCenterOptions
  ) {
    this.locations = locations;
    this.options = { ...Position.defaultCenterOptions, ...options };
  }

  /**
   * Adds a location to the set of points.
   *
   * @name Position#add
   * @function
   * @param {Array} location 2D points on a plane
   */
  add(location: Array<number>): void {
    this.locations.push(location);
  }

  /**
   * Removes a location to the set of points.
   *
   * @name Position#remove
   * @function
   * @param {Array} location 2D points on a plane
   * @return {Array|number} The removed location, or `-1` if no match is found
   */
  remove(location: Array<number>): Array<number> | number {
    for (let i = 0; i < this.locations.length; i++) {
      if (
        this.locations[i][0] === location[0] &&
        this.locations[i][1] === location[1]
      ) {
        this.locations.splice(i, 1);
        return location;
      }
    }
    return -1;
  }

  /**
   * Calculates the geometric center of the Position.
   *
   * @name Position#center
   * @see Center#geometric
   * @function
   * @return {Array} Geometric center of the Position
   */
  get center(): Array<number> {
    return Center.geometric(this.locations, ...this.optionValues).center;
  }

  /**
   * Calculates the median (center of mass) of the Position.
   *
   * @name Position#median
   * @see Center#mass
   * @desc a rudimentary estimate for Position#center
   * @function
   * @return {Array} Geometric center of the Position
   */
  get median(): Array<number> {
    return Center.mass(this.locations).center;
  }

  /**
   * Calculates the decimal improvement of Position#center as compared to
   * Position#median in each dimension.
   *
   * @name Position#score
   * @function
   * @return {Array} Median of the Position
   */
  get score(): number {
    const [median, center] = [
      Center.mass(this.locations).score,
      Center.geometric(this.locations, ...this.optionValues).score
    ];
    return (median - center) / median;
  }

  /**
   * Returns the values of each key of the current Position object
   *
   * @function
   * @private
   * @return {Array} Values of each key of the Position
   */
  private get optionValues(): Array<number | boolean> {
    return Object.getOwnPropertyNames(this.options).map(v => this.options[v]);
  }
}
