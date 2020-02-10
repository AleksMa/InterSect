#ifndef INTERSECT_PARABOLOIDELLIPTIC_H
#define INTERSECT_PARABOLOIDELLIPTIC_H

#include <vector>
#include "../Primary/Point.h"
#include "AbstractSurface.h"

using namespace std;

class ParaboloidElliptic : public AbstractSurface {
private:
    int N, M;

    float A, B;

    vector<Point> make_vertices();

public:
    ParaboloidElliptic(float a, float b);

    vector<Point> make_mash() override;

    float max_z() override;
    float min_z() override;
};


#endif //INTERSECT_PARABOLOIDELLIPTIC_H
