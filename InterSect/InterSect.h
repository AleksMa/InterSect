#ifndef INTERSECT_INTERSECT_H
#define INTERSECT_INTERSECT_H

#include "InterSect.h"
#include "../Primary/Point.h"
#include "../SurfaceEquations/SurfaceEquation.h"
#include "../Surfaces/AbstractSurface.h"
#include "../Evaluations/Calculations.cpp"
#include "../Evaluations/Equation.h"
#include "../Evaluations/Matrix.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class InterSect {
private:
    shared_ptr<SurfaceEquation> first_equation;
    shared_ptr<SurfaceEquation> second_equation;

    shared_ptr<AbstractSurface> first_surface;
    shared_ptr<AbstractSurface> second_surface;

    vector<Point> make_intersect();
    vector<Point> make_ph_intersect();

    float z_step_ext = 0.1;
    float y_step_ext = 0.01;

    float z_step = 0.5;
    float y_step = 0.1;
public:
    InterSect(shared_ptr<SurfaceEquation> first_equation, shared_ptr<SurfaceEquation> second_equation,
              shared_ptr<AbstractSurface> first_surface, shared_ptr<AbstractSurface> second_surface);

    vector<Point> create_intersect();
};


#endif //INTERSECT_INTERSECT_H
