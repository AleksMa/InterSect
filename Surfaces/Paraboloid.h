#ifndef INTERSECT_PARABOLOID_H
#define INTERSECT_PARABOLOID_H

#include <vector>
#include "../Primary/Point.h"
using namespace std;

class Paraboloid {
public:
    float A, B;

    int N, M;

    Paraboloid(float a, float b);

    vector<Point> makeVertices();
    vector<Point> makeMash();

};


#endif //INTERSECT_PARABOLOID_H
