import { Position } from './position';
import { createClient } from '@google/maps';
import {
  GoogleMapsClient,
  PlacesOptions,
  CenterOptions
} from './interfaces/index';

/**
 * A prototype describing a set of points on a map, with first-class Google Maps
 * integration. Non-trivial tasks require the declaration of an API token.
 *
 * ```
 * import { MeetHere } from 'meethere';
 *
 * let Map = new MeetHere(
 *   [ [-33, 44], [-35, 41], [-31, 43] ],
 *   MY_GOOGLE_MAPS_TOKEN
 * );
 * ```
 *
 * In general, calls to Google-esque services will return an asynchronous
 * `Promise` from which the response can be handled.
 *
 * ```
 * Map.nearby() // => Promise<pending>
 * Map.nearby(options, true).then(processNearbyLocations)
 *
 * function processNearbyLocations(error, result) {
 *   ...
 * }
 * ```
 *
 * @class
 * @extends Position
 */
export class MeetHere extends Position {
  client: GoogleMapsClient;

  /**
   * Default nearby search options
   *
   * @constant
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#placesNearby
   * @type {object}
   * @default
   */
  static defaultPlacesOptions: PlacesOptions = {
    language: 'English',
    rankby: 'distance'
  };

  /**
   * Default geometric center options
   *
   * @constant
   * @type {SearchOptions}
   * @default
   */
  static defaultCenterOptions: CenterOptions = {
    subsearch: true,
    epsilon: 1e-4,
    bounds: 10
  };

  /**
   * Creates a MeetHere on a map described by a set of locations.
   *
   * @see https://github.com/googlemaps/google-maps-services-js/tree/master#api-keys
   * @constructs
   * @param {Array} locations 2D Array of points on a map
   * @param {string} token Google Maps API token
   * @param {CenterOptions} [options=MeetHere.defaultCenterOptions] Whether to search for centroid obliquely
   */
  constructor(
    locations: Array<Array<number>>,
    token: string,
    options: CenterOptions = MeetHere.defaultCenterOptions
  ) {
    super(locations, options);
    this.client = createClient({ key: token, Promise: Promise });
  }

  /**
   * Same as MeetHere#center.
   *
   * @name MeetHere#meetHere
   * @alias Position#center
   * @function
   * @return {Array} Geometric center of the Position
   */
  get meetHere(): Array<number> {
    return super.center;
  }

  /**
   * Returns roads near the center of the MeetHere.
   *
   * @name MeetHere#roads
   * @function
   * @param {boolean} [geometric=true] Whether to use geometric/median center
   * @return {Promise} A Promise that will yield nearby roads or an error
   */
  roads(geometric: boolean = true): Promise<object> {
    const center = this.middle(geometric);
    return this.client
      .nearestRoads({ points: [center] })
      .asPromise()
      .then(response => response.json)
      .catch(error => error.json);
  }

  /**
   * Returns places near the center of the MeetHere.
   *
   * @name MeetHere#nearby
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#placesNearby
   * @function
   * @param {PlacesOptions} [options=defaultSearchOptions] Options to apply to
   * search request, can be any of @see
   * @param {boolean} [geometric=true] Whether to use geometric/median center
   * @return {Promise} A Promise that will yield nearby places or an error
   */
  nearby(
    options: PlacesOptions = MeetHere.defaultPlacesOptions,
    geometric: boolean = true
  ): Promise<object> {
    options['location'] = this.middle(geometric);
    return this.client
      .placesNearby(options)
      .asPromise()
      .then(response => response.json)
      .catch(error => error.json);
  }

  /**
   * Returns the center of the MeetHere based on the geometric parameter.
   *
   * @function
   * @private
   * @param {boolean} geometric Whether to use geometric/median center
   * @return {Array} The center of the MeetHere
   */
  private middle(geometric: boolean): Array<number> {
    return geometric ? super.center : super.median;
  }
}
