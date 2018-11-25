#ifndef CARTESIAN_H
#define CARTESIAN_H

struct cartesian
{
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
  double (*haversine_distance)(double start_latitude,
                               double start_longitude,
                               double end_latitude,
                               double end_longitude,
                               char   unit);
};

extern const struct cartesian Cartesian;

#endif
