#ifndef INTERSECT_HYPERBOLOIDONESHEET_H
#define INTERSECT_HYPERBOLOIDONESHEET_H


#include "AbstractSurface.h"

class HyperboloidOneSheet : public AbstractSurface {
private:
    int N, M, size;

    float A, B, C;

    vector<Point> make_vertices();

public:
    HyperboloidOneSheet(float a, float b, float c);

    vector<Point> make_mash() override;

    float max_z() override;
    float min_z() override;
};


#endif //INTERSECT_HYPERBOLOIDONESHEET_H
