//
// Created by alexey on 30.01.2020.
//

#ifndef INTERSECT_ABSTRACTSURFACE_H
#define INTERSECT_ABSTRACTSURFACE_H


#include "SurfaceEquation.h"
typedef vector<float> VF;

enum surface_type {
    UNKNOWN,
    ELLIPSOID,
    PARABOLOID_ELLIPTIC,
};

class AbstractSurface {
private:
    surface_type type;

    SurfaceEquation equation;
    SurfaceEquation temporary;
    SurfaceEquation canonical_equation;

    VF additional_vector;
    vector<VF> mul_matrix;

    vector<float> getEigenvalues();

public:
    explicit AbstractSurface(VF coef);
    AbstractSurface();

    SurfaceEquation canonizate();

    SurfaceEquation get_canonical;

    vector<VF> get_mul_matrix();
    VF get_additional_vector();

    surface_type get_type();
};


#endif //INTERSECT_ABSTRACTSURFACE_H
