#include <math.h>

#define f64 double

static f64 square(f64 A) {
  f64 Result = (A * A);
  return Result;
}

static f64 radians_from_degrees(f64 Degrees) {
  f64 Result = 0.01745329251994329577 * Degrees;
  return Result;
}

f64 reference_haversine(f64 X0, f64 Y0, f64 X1, f64 Y1, f64 EarthRadius) {
  f64 lat1 = Y0;
  f64 lat2 = Y1;
  f64 lon1 = X0;
  f64 lon2 = X1;

  f64 dLat = radians_from_degrees(lat2 - lat1);
  f64 dLon = radians_from_degrees(lon2 - lon1);
  lat1 = radians_from_degrees(lat1);
  lat2 = radians_from_degrees(lat2);

  f64 a =
      square(sin(dLat / 2.0)) + cos(lat1) * cos(lat2) * square(sin(dLon / 2));
  f64 c = 2.0 * asin(sqrt(a));

  f64 Result = EarthRadius * c;

  return Result;
}
