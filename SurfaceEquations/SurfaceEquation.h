#ifndef INTERSECT_SURFACEEQUATION_H
#define INTERSECT_SURFACEEQUATION_H


#include "QuadricEquation.h"
typedef vector<float> VF;

enum surface_type {
    UNKNOWN,
    ELLIPSOID,
    PARABOLOID_ELLIPTIC,
    PARABOLOID_HYPERBOLIC,
    HYPERBOLOID_ONE_SHEET,
    HYPERBOLOID_TWO_SHEET,
};

class SurfaceEquation {
private:
    surface_type type;

    QuadricEquation equation;
    QuadricEquation temporary;
    QuadricEquation canonical_equation;

    VF additional_vector;
    vector<VF> mul_matrix;

    vector<float> getEigenvalues();

public:
    explicit SurfaceEquation(VF coef);
    SurfaceEquation();

    QuadricEquation canonizate();

    QuadricEquation get_canonical();
    QuadricEquation get_equation();

    vector<VF> get_mul_matrix();
    VF get_additional_vector();

    surface_type get_type();
};


#endif //INTERSECT_SURFACEEQUATION_H
