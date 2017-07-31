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
 * Describes a CenterOptions Object
 *
 * @interface
 */
export interface CenterOptions {
  subsearch?: boolean;
  epsilon?: number;
  bounds?: number;
}

/**
 * Describes a DistanceOptions Object
 *
 * @interface
 */
export interface DistanceOptions {
  mode?: string;
  language?: string;
  avoid?: Array<string>;
  units?: string;
  departure_time?: Date | number;
  arrival_time?: Date | number;
  transit_mode?: Array<string>;
  transit_routing_preference?: string;
  traffic_model?: string;
}

/**
 * Describes a PlacesOptions Object
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
 * Describes a TimeZoneOptions Object
 *
 * @interface
 */
export interface TimeZoneOptions {
  timestamp?: Date | number;
  language?: string;
}
