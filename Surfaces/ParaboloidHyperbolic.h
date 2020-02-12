#ifndef INTERSECT_PARABOLOIDHYPERBOLIC_H
#define INTERSECT_PARABOLOIDHYPERBOLIC_H


#include "AbstractSurface.h"

class ParaboloidHyperbolic : public AbstractSurface {
private:
    int N, M, size_x, size_z, K;

    float A, B;

    float max, min;

    vector<Point> make_vertices();

public:
    ParaboloidHyperbolic(float a, float b);

    vector<Point> make_mash() override;

    float max_z() override;
    float min_z() override;
};


#endif //INTERSECT_PARABOLOIDHYPERBOLIC_H
