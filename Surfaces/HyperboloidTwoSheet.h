//
// Created by alexey on 05.02.2020.
//

#ifndef INTERSECT_HYPERBOLOIDTWOSHEET_H
#define INTERSECT_HYPERBOLOIDTWOSHEET_H


#include "AbstractSurface.h"

class HyperboloidTwoSheet : public AbstractSurface {
private:
    int N, M, size, K;

    float A, B, C;

    vector<Point> make_vertices();

public:
    HyperboloidTwoSheet(float a, float b, float c);

    vector<Point> make_mash() override;

    float max_z() override;
    float min_z() override;
};


#endif //INTERSECT_HYPERBOLOIDTWOSHEET_H
