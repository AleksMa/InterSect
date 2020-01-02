//
// Created by alexey on 18.12.2019.
//

#ifndef INTERSECT_ELLIPSOID_H
#define INTERSECT_ELLIPSOID_H

#include <vector>
#include "../Primary/Point.h"
using namespace std;

class Ellipsoid {
public:
    float A, B, C, D;

    int N, M;

    Ellipsoid(float a, float b, float c, float d);

    vector<Point> makeEllipsoidVertices();
    vector<Point> makeEllipsoidMash();

};


#endif //INTERSECT_ELLIPSOID_H
