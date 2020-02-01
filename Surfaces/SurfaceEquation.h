//
// Created by alexey on 30.01.2020.
//

#ifndef INTERSECT_SURFACEEQUATION_H
#define INTERSECT_SURFACEEQUATION_H

#include <vector>

using namespace std;

typedef vector<float> VF;


struct SurfaceEquation {
    const int size = 10;
    vector<float> equation;
    explicit SurfaceEquation(VF coef);
    explicit SurfaceEquation();
    float &XX();
    float &YY();
    float &ZZ();
    float &XY();
    float &XZ();
    float &YZ();
    float &X();
    float &Y();
    float &Z();
    float &D();
    void print();
};


#endif //INTERSECT_SURFACEEQUATION_H
