import { Position } from './position';
import { createClient } from '@google/maps';
import {
  GoogleMapsClient,
  CenterOptions,
  DistanceOptions,
  PlacesOptions,
  TimeZoneOptions
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
   * Default geometric center options
   *
   * @constant
   * @type {CenterOptions}
   * @default
   */
  static defaultCenterOptions: CenterOptions = {
    subsearch: true,
    epsilon: 1e-4,
    bounds: 10
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
    rankby: 'distance'
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
    timestamp: null
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
   * Returns a distance matrix from each location to the center of the MeetHere.
   *
   * @name MeetHere#distance
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#distanceMatrix
   * @function
   * @param {DistanceOptions} [options=MeetHere.defaultDistanceOptions] Options
   * to apply to distance matrix request, can be any of @see
   * @param {boolean} [geometric=true] Whether to use geometric or median center
   * @return {Promise} A Promise that will yield the distance matrix or an error
   */
  distance(
    options: DistanceOptions = {},
    geometric: boolean = true
  ): Promise<object> {
    options['origins'] = this.locations;
    options['destinations'] = [this.middle(geometric)];
    return this.client
      .distanceMatrix({
        ...MeetHere.defaultDistanceOptions,
        ...options
      })
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
   * @param {PlacesOptions} [options=MeetHere.defaultPlacesOptions] Options to
   * apply to search request, can be any of @see
   * @param {boolean} [geometric=true] Whether to use geometric or median center
   * @return {Promise} A Promise that will yield nearby places or an error
   */
  nearby(
    options: PlacesOptions = {},
    geometric: boolean = true
  ): Promise<object> {
    options['location'] = this.middle(geometric);
    return this.client
      .placesNearby({ ...MeetHere.defaultPlacesOptions, ...options })
      .asPromise()
      .then(response => response.json)
      .catch(error => error.json);
  }

  /**
   * Returns roads near the center of the MeetHere.
   *
   * @name MeetHere#roads
   * @function
   * @param {boolean} [geometric=true] Whether to use geometric or median center
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
   * Returns the time offset data of the center of the MeetHere at some moment.
   *
   * @name MeetHere#timezone
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#timezone
   * @function
   * @param {TimeZoneOptions} [options=MeetHere.defaultTimeZoneOptions] Options
   * to apply to timezone request, can be any of @see
   * @param {boolean} [geometric=true] Whether to use geometric or median center
   * @return {Promise} A Promise that will yield time offset data or an error
   */
  timezone(
    options: TimeZoneOptions = {},
    geometric: boolean = true
  ): Promise<object> {
    options['location'] = this.middle(geometric);
    options = { ...MeetHere.defaultTimeZoneOptions, ...options };
    options['timestamp'] = options['timestamp'] || ~~(Date.now() / 1000);
    return this.client
      .timezone(options)
      .asPromise()
      .then(response => response.json)
      .catch(error => error.json);
  }
}
