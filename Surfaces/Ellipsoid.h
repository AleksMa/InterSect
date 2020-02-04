//
// Created by alexey on 18.12.2019.
//

#ifndef INTERSECT_ELLIPSOID_H
#define INTERSECT_ELLIPSOID_H

#include <vector>
#include "../Primary/Point.h"
#include "AbstractSurface.h"

using namespace std;

class Ellipsoid : public AbstractSurface {
private:
    int N, M;

    float A, B, C;

    vector<Point> make_vertices();

public:
    Ellipsoid(float a, float b, float c);

    vector<Point> make_mash() override;

};


#endif //INTERSECT_ELLIPSOID_H
