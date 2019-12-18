//
// Created by alexey on 18.12.2019.
//

#ifndef INTERSECT_ELLIPSOID_H
#define INTERSECT_ELLIPSOID_H

#include <vector>
#include "Point.h"
using namespace std;

class Ellipsoid {
public:
    float A, B, C;
    float XX, YY, ZZ, X, Y, Z, XY, XZ, YZ, D;
    float cX, cY, cZ;
    int R;

    int N, M;

    Ellipsoid(float a, float b, float c, float d);

    Ellipsoid(float a, float b, float c, float cX, float cY, float cZ, int R, int n, int m);

    vector<Point> makeEllipsoidVerticies();
    vector<Point> makeEllipsoidMash();

};


#endif //INTERSECT_ELLIPSOID_H
