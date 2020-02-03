//
// Created by alexey on 29.01.2020.
//

#ifndef INTERSECT_EQUATION_CPP
#define INTERSECT_EQUATION_CPP

#pragma once

#include "Equation.h"
#include "Calculations.cpp"

#include <utility>
#include <iostream>

#define M_2PI (2.f*M_PI)

typedef vector<float> VF;

Equation::Equation(vector<float> coef) : equation(std::move(coef)) {
    while (equation.size() < 4)
        equation.push_back(0);
    if (!is_zero(equation[3])) {
        for (int i = 0; i < 3; ++i) {
            equation[i] /= equation[3];
        }
        equation[3] = 1.f;
    }
    //cout << equation[3] << "X^3 + " << equation[2] << "X^2 + " << equation[1] << "X + " << equation[0] << endl << endl;
}

vector<float> Equation::solve() {
    if (is_zero(equation[3])) {
        return solve_sqr(true);
    } else if (is_zero(equation[0])) {
        auto solved = solve_sqr(false);
        solved.push_back(0);
        return solved;
    } else
        return solve_cube();
}

vector<float> Equation::solve_sqr(bool from_digits) {
    int i = 0;
    if (!from_digits)
        i++;
    float C = equation[i], B = equation[i + 1], A = equation[i + 2];
    float D = B * B - 4 * A * C;
    if (less_zero(D))
        return vector<float>();
    else if (is_zero(D)) {
        float X = -B / (2 * A);
        return vector<float>{X, X};
    } else {
        float X1 = (-B + sqrt(D)) / (2 * A);
        float X2 = (-B - sqrt(D)) / (2 * A);
        return vector<float>{X1, X2};
    }
}

vector<float> Equation::solve_cube() {
    auto x = new float[3];
    int count = solve_cube_helper(x);
    vector<float> res;

    if (count == 3)
        res = VF{x[0], x[1], x[2]};
    else if (count == 2)
        res = VF{x[0], x[1], x[1]};

    delete[] x;
    return res;
}


int Equation::solve_cube_helper(float *x) {
    float a = equation[2], b = equation[1], c = equation[0];
    float q, r, r2, q3;
    q = (a * a - 3.f * b) / 9.f;
    r = (a * (2.f * a * a - 9.f * b) + 27.f * c) / 54.f;
    r2 = r * r;
    q3 = q * q * q;
    if (!equal(r2, q3) && r2 < q3) {
        float t = acos(r / sqrt(q3));
        a /= 3.f;
        q = -2.f * sqrt(q);
        x[0] = q * cos(t / 3.f) - a;
        x[1] = q * cos((t + M_2PI) / 3.f) - a;
        x[2] = q * cos((t - M_2PI) / 3.f) - a;
        return 3;
    } else {
        float aa, bb;
        if (r <= 0.f || is_zero(r)) r = -r;
        aa = cbrt(r + sqrt(r2 - q3));
        if (!is_zero(aa))
            bb = q / aa;
        else
            bb = 0.f;
        a /= 3.f;
        q = aa + bb;
        r = aa - bb;
        x[0] = q - a;
        x[1] = (-0.5f) * q - a;
        x[2] = (sqrt(3.f) * 0.5f) * fabs(r);
        if (is_zero(x[2]))
            return 2;
        return 1;
    }
}


#endif