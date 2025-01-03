#ifndef HARVESTINE_H_
#define HARVESTINE_H_

#define REFERENCE_EARTH_RADIUS 6372.8

double reference_haversine(double X0, double Y0, double X1, double Y1,
                           double EarthRadius);

#endif // HARVESTINE_H_
