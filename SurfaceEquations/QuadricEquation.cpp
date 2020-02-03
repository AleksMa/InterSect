//
// Created by alexey on 30.01.2020.
//

#include "QuadricEquation.h"

#include <utility>
#include <iostream>

QuadricEquation::QuadricEquation(VF coef): equation(std::move(coef)) {
    while (equation.size() < size)
        equation.push_back(0.);
}

QuadricEquation::QuadricEquation(): equation(VF(10, 0.)) {}

float &QuadricEquation::XX() {
    return equation[0];
}

float &QuadricEquation::YY() {
    return equation[1];
}

float &QuadricEquation::ZZ() {
    return equation[2];
}

float &QuadricEquation::XY() {
    return equation[3];
}

float &QuadricEquation::XZ() {
    return equation[4];
}

float &QuadricEquation::YZ() {
    return equation[5];
}

float &QuadricEquation::X() {
    return equation[6];
}

float &QuadricEquation::Y() {
    return equation[7];
}

float &QuadricEquation::Z() {
    return equation[8];
}

float &QuadricEquation::D() {
    return equation[9];
}

void QuadricEquation::print() {
    for (auto el : equation) {
        cout << el << " ";
    }
    cout << endl;
}

void QuadricEquation::mul(float m) {
    for (float & el : equation) {
        el *= m;
    }
}
