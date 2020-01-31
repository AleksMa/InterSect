//
// Created by alexey on 30.01.2020.
//

#ifndef INTERSECT_ABSTRACTSURFACE_H
#define INTERSECT_ABSTRACTSURFACE_H


#include "SurfaceEquation.h"
typedef vector<float> VF;

class AbstractSurface {
private:
    SurfaceEquation equation;
    SurfaceEquation temporary;
    SurfaceEquation canonical;

    vector<float> getEigenvalues();

public:
    explicit AbstractSurface(VF coef);
    AbstractSurface();

    void getCanonical();
};


#endif //INTERSECT_ABSTRACTSURFACE_H
