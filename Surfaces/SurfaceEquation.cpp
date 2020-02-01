//
// Created by alexey on 30.01.2020.
//

#include "SurfaceEquation.h"

#include <utility>
#include <iostream>

SurfaceEquation::SurfaceEquation(VF coef): equation(std::move(coef)) {
    while (equation.size() < size)
        equation.push_back(0.);
}

SurfaceEquation::SurfaceEquation(): equation(VF(10, 0.)) {}

float &SurfaceEquation::XX() {
    return equation[0];
}

float &SurfaceEquation::YY() {
    return equation[1];
}

float &SurfaceEquation::ZZ() {
    return equation[2];
}

float &SurfaceEquation::XY() {
    return equation[3];
}

float &SurfaceEquation::XZ() {
    return equation[4];
}

float &SurfaceEquation::YZ() {
    return equation[5];
}

float &SurfaceEquation::X() {
    return equation[6];
}

float &SurfaceEquation::Y() {
    return equation[7];
}

float &SurfaceEquation::Z() {
    return equation[8];
}

float &SurfaceEquation::D() {
    return equation[9];
}

void SurfaceEquation::print() {
    for (auto el : equation) {
        cout << el << " ";
    }
    cout << endl;
}
