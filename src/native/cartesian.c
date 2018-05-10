#include "cartesian.h"

#include "toolkit/matrix.h"

#include <math.h>

//
// HELPERS
//

static const float EARTH_RADIUS_METERS = 6371e3;
static const float METER_TO_KM         = 1e-3;
static const float METER_TO_MI         = 6.2137119223733e-4;

//
// MAIN
//

/**
 * @brief   Calculates the earthly distance between two cartesian points.
 * @details Uses the Haversine formula to calculate the distance between two
 *          Latitude/Longitude points
 *            a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
 *            c = 2 ⋅ atan2( √a, √(1−a) )
 *            d = R ⋅ c
 *          where
 *            φ1 - starting latitude
 *            φ2 - ending latitude
 *            Δφ - difference in latitudes
 *            Δλ - difference in longitudes.
 * @note    All measurements must be in radians.
 *
 * @param   start_latitude   starting latitude
 * @param   start_longitude  starting longitude
 * @param   end_latitude     ending latitude
 * @param   end_longitude    ending longitude
 * @param   unit             type of unit to use: meters or miles
 *
 * @return  distance between two cartesian points
 */
static double haversine_distance(const double start_latitude,
                                 const double start_longitude,
                                 const double end_latitude,
                                 const double end_longitude,
                                 const char   unit)
{
  const double delta_latitude  = end_latitude - start_latitude;
  const double delta_longitude = end_longitude - start_longitude;

  const double a = pow(sin(delta_latitude / 2), 2) +
                   cos(start_latitude) * cos(end_latitude) *
                       pow(sin(delta_longitude / 2), 2);
  const double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  const double d = c * EARTH_RADIUS_METERS;

  return d * (unit == 'm' ? METER_TO_KM : METER_TO_MI);
}

//
// WRAPPERS
//

const struct cartesian Cartesian = {.haversine_distance = haversine_distance};
