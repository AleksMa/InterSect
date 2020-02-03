#ifndef INTERSECT_PARABOLOID_H
#define INTERSECT_PARABOLOID_H

#include <vector>
#include "../Primary/Point.h"
#include "AbstractSurface.h"

using namespace std;

class Paraboloid : public AbstractSurface {
private:
    float A, B;

    int N, M;

public:
    Paraboloid(float a, float b);

    vector<Point> makeVertices();

    vector<Point> makeMash();

};


#endif //INTERSECT_PARABOLOID_H
