//
// Created by alexey on 29.01.2020.
//

#ifndef INTERSECT_EQUATION_H
#define INTERSECT_EQUATION_H

#pragma once
#include <vector>

using namespace std;


class Equation {
private:
    vector<float> equation;

    vector<float> solve_sqr(bool from_digits);
    vector<float> solve_cube();
    int solve_cube_helper(float *x);

public:
    Equation(vector<float> coef);

    vector<float> solve();
};


#endif //INTERSECT_EQUATION_H
