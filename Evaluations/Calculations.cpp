#ifndef INTERSECT_CALCULATION
#define INTERSECT_CALCULATION

#include <math.h>

#define epsi 1e-3;

static bool is_zero(float num) { return abs(num) < epsi; }

static bool equal(float left, float right) { return abs(left - right) < epsi; }

static bool less_zero(float num) { return !is_zero(num) && num < -epsi; }

static bool is_zero_eps(float num, float epsilon) { return abs(num) < epsilon; }

static bool equal_eps(float left, float right, float epsilon) { return abs(left - right) < epsilon; }

#endif //INTERSECT_CALCULATION