import { Position, CLIB } from './position';
import { createClient } from '@google/maps';
import {
  GoogleMapsClient,
  CenterOptions,
  DistanceOptions,
  PlacesOptions,
  TimeZoneOptions,
} from './interfaces/index';
const unitType = {
  km: 'k'.charCodeAt(0),
  mi: 'm'.charCodeAt(0),
};

/**
 * A prototype describing a set of points on a map, with first-class Google Maps
 * integration. Declaration of an API token is not required for use of the
 * class, but any API requests will fail without one.
 *
 * ```
 * import { MeetHere } from 'meethere';
 *
 * let map = new MeetHere(
 *   [[-33, 44], [-35, 41], [-31, 43]],
 *   MY_GOOGLE_MAPS_TOKEN
 * );
 * ```
 *
 * In general, calls to Google-esque services will return an asynchronous
 * `Promise` from which the response can be handled.
 *
 * ```
 * map.nearby() // => Promise<pending>
 * map.nearby(options, true).then(processNearbyLocations)
 *
 * function processNearbyLocations(error, result) {
 *   ...
 * }
 * ```
 *
 * @class
 * @extends Position
 */
class MeetHere extends Position {
  client: GoogleMapsClient;

  /**
   * Default geometric center options
   *
   * @constant
   * @type {CenterOptions}
   * @default
   */
  static defaultCenterOptions: CenterOptions = {
    subsearch: true,
    epsilon: 1e-4,
    bounds: 10,
    startIndex: 0,
    degree: null,
  };

  /**
   * Default distance matrix options
   *
   * @constant
   * @type {DistanceOptions}
   * @default
   */
  static defaultDistanceOptions: DistanceOptions = {};

  /**
   * Default nearby search options
   *
   * @constant
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#placesNearby
   * @type {PlacesOptions}
   * @default
   */
  static defaultPlacesOptions: PlacesOptions = {
    language: 'English',
    rankby: 'distance',
  };

  /**
   * Default timezone search options
   *
   * @constant
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#timezone
   * @type {TimeZoneOptions}
   * @default
   */
  static defaultTimeZoneOptions: TimeZoneOptions = {
    timestamp: null,
  };

  /**
   * Creates a MeetHere on a map described by a set of locations.
   *
   * @see https://github.com/googlemaps/google-maps-services-js/tree/master#api-keys
   * @constructs
   * @param {Array} locations 2D Array of points on a map
   * @param {string} token Google Maps API token
   * @param {CenterOptions} [options=MeetHere.defaultCenterOptions] Whether to
   * search for centroid obliquely
   */
  constructor(
    locations: Array<Array<number>>,
    token: string,
    options: CenterOptions = {},
  ) {
    super(locations, { ...MeetHere.defaultCenterOptions, ...options });
    this.client = createClient({ key: token, Promise: Promise });
  }

  /**
   * Returns the center of the MeetHere based on the geometric parameter.
   *
   * @function
   * @private
   * @param {boolean} geometric Whether to use geometric/mean center
   * @return {Array} The center of the MeetHere
   */
  private middle(geometric: boolean): Array<number> {
    return geometric ? this.center : this.mean;
  }

  /**
   * Same as MeetHere#center.
   *
   * @name MeetHere#meetHere
   * @alias Position#center
   * @function
   * @return {Array} Geometric center of the Position
   *
   * ```
   * let map = new MeetHere([[-33, 44], [-35, 41], [-31, 43]]);
   * map.meetHere; // => [-32.80928, 43.39817]
   * ```
   */
  get meetHere(): Array<number> {
    return this.center;
  }

  /**
   * Returns a distance matrix from each point of the MeetHere to the geometric
   * center.
   *
   * @name MeetHere#distance
   * @function
   * @param {string} [units='km'] Units of distance to use, can be 'km' or 'mi'
   * @param {boolean} [geometric=true] Whether to use geometric center or mean
   * @return {Object.<string, Array>} A Promise that will yield distances or
   * an error
   *
   * ```
   * let map = new MeetHere([[-33, 44], [-35, 41], [-31, 43]]);
   * map.distance; // => { origins: [[-33, 44], [-35, 41], [-31, 43]],
   *               //      destination: [-32.80928, 43.39817],
   *               //      distances: [37.31571, 204.49320, 127.17181] }
   * ```
   */
  distanceMatrix(
    units: string = 'km',
    geometric: boolean = true,
  ): {
    origins: Array<number>;
    destinations: Array<number>;
    distances: Array<number>;
  } {
    return CLIB.distance(
      this.locations,
      this.middle(geometric),
      unitType[units],
    );
  }

  /**
   * Returns places near the center of the MeetHere.
   *
   * @name MeetHere#nearby
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#placesNearby
   * @function
   * @async
   * @param {PlacesOptions} [options=MeetHere.defaultPlacesOptions] Options to
   * apply to search request, can be any of @see
   * @param {boolean} [geometric=true] Whether to use geometric center or mean
   * @return {Promise} A Promise that will yield nearby places or an error
   */
  async nearby(
    options: PlacesOptions = {},
    geometric: boolean = true,
  ): Promise<object> {
    options['location'] = this.middle(geometric);
    return await this.client
      .placesNearby({ ...MeetHere.defaultPlacesOptions, ...options })
      .asPromise()
      .then((response) => response.json)
      .catch((error) => error.json);
  }

  /**
   * Returns roads near the center of the MeetHere.
   *
   * @name MeetHere#roads
   * @function
   * @async
   * @param {boolean} [geometric=true] Whether to use geometric center or mean
   * @return {Promise} A Promise that will yield nearby roads or an error
   */
  async roads(geometric: boolean = true): Promise<object> {
    const center = this.middle(geometric);
    return await this.client
      .nearestRoads({ points: [center] })
      .asPromise()
      .then((response) => response.json)
      .catch((error) => error.json);
  }

  /**
   * Returns the time offset data of the center of the MeetHere at some moment.
   *
   * @name MeetHere#timezone
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#timezone
   * @function
   * @async
   * @param {TimeZoneOptions} [options=MeetHere.defaultTimeZoneOptions] Options
   * to apply to timezone request, can be any of @see
   * @param {boolean} [geometric=true] Whether to use geometric center or mean
   * @return {Promise} A Promise that will yield time offset data or an error
   */
  async timezone(
    options: TimeZoneOptions = {},
    geometric: boolean = true,
  ): Promise<object> {
    options['location'] = this.middle(geometric);
    options = { ...MeetHere.defaultTimeZoneOptions, ...options };
    options['timestamp'] = options['timestamp'] || ~~(Date.now() / 1000);
    return await this.client
      .timezone(options)
      .asPromise()
      .then((response) => response.json)
      .catch((error) => error.json);
  }

  /**
   * Returns a matrix from each location to the center of the MeetHere with
   * distance and time fields.
   *
   * @name MeetHere#travel
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#distanceMatrix
   * @function
   * @async
   * @param {DistanceOptions} [options=MeetHere.defaultDistanceOptions] Options
   * to apply to distance matrix request, can be any of @see
   * @param {boolean} [geometric=true] Whether to use geometric center or mean
   * @return {Promise} A Promise that will yield the distance matrix or an error
   */
  async travel(
    options: DistanceOptions = {},
    geometric: boolean = true,
  ): Promise<object> {
    options['origins'] = this.locations;
    options['destinations'] = [this.middle(geometric)];
    return await this.client
      .distanceMatrix({
        ...MeetHere.defaultDistanceOptions,
        ...options,
      })
      .asPromise()
      .then((response) => response.json)
      .catch((error) => error.json);
  }
}

export { MeetHere };
