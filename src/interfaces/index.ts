/**
 * Describes a Google Maps client
 *
 * @interface
 */
export interface GoogleMapsClient {
  directions: Function;
  distanceMatrix: Function;
  elevation: Function;
  elevationAlongPath: Function;
  geocode: Function;
  geolocate: Function;
  reverseGeocode: Function;
  places: Function;
  placesNearby: Function;
  placesRadar: Function;
  place: Function;
  placesPhoto: Function;
  placesAutoComplete: Function;
  placesQueryAutoComplete: Function;
  snapToRoads: Function;
  nearestRoads: Function;
  speedLimits: Function;
  snappedSpeedLimits: Function;
  timezone: Function;
}

/**
 * Describes MeetHere#nearby options
 *
 * @interface
 */
export interface PlacesOptions {
  language?: string;
  radius?: number;
  keyword?: string;
  minprice?: number;
  maxprice?: number;
  name?: string;
  opennow?: boolean;
  rankby?: string;
  type?: string;
  pagetoken?: string;
}

/**
 * Describes a SearchOptions Object
 *
 * @interface
 */
export interface CenterOptions {
  subsearch?: boolean;
  epsilon?: number;
  bounds?: number;
}
