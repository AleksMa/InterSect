#ifndef INTERSECT_ABSTRACTSURFACE_H
#define INTERSECT_ABSTRACTSURFACE_H

#include "../SurfaceEquations/SurfaceEquation.h"
#include "../Primary/Point.h"

class AbstractSurface {
public:
    virtual vector<Point> make_mash() = 0;

    virtual float max_z() = 0;
    virtual float min_z() = 0;
};

#endif //INTERSECT_ABSTRACTSURFACE_H
