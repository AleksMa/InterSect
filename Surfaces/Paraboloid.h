#ifndef INTERSECT_PARABOLOID_H
#define INTERSECT_PARABOLOID_H

#include <vector>
#include "../Primary/Point.h"
#include "AbstractSurface.h"

using namespace std;

class Paraboloid : public AbstractSurface {
private:
    int N, M;

    float A, B;

    vector<Point> make_vertices();

public:
    Paraboloid(float a, float b);

    vector<Point> make_mash() override;

};


#endif //INTERSECT_PARABOLOID_H
