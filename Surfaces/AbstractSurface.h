//
// Created by alexey on 03.02.2020.
//

#ifndef INTERSECT_ABSTRACTSURFACE_H
#define INTERSECT_ABSTRACTSURFACE_H

#include "../SurfaceEquations/SurfaceEquation.h"
#include "../Primary/Point.h"

class AbstractSurface {
protected:
    //surface_type type = UNKNOWN;

public:
    virtual vector<Point> makeVertices() = 0;
    virtual vector<Point> makeMash() = 0;

};

#endif //INTERSECT_ABSTRACTSURFACE_H
