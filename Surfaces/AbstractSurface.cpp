//
// Created by alexey on 30.01.2020.
//

#include "AbstractSurface.h"
#include "../Evaluations/Equation.h"
#include "../Evaluations/Calculations.cpp"

AbstractSurface::AbstractSurface(VF coef) : equation(coef), canonical(VF(10)) {}

AbstractSurface::AbstractSurface() : equation(VF(10)), canonical(VF(10)) {}

void AbstractSurface::getCanonical() {
    // mathhelpplanet.com/static.php?p=privedenie-uravneniya-poverhnosti-k-kanonicheskomu-vidu
    // 1
    if (is_zero(equation.XY()) &&
        is_zero(equation.XZ()) &&
        is_zero(equation.YZ())) {
        // МАТРИЦА ДИАГОНАЛЬНА => К ШАГУ 4
    }
    // 2
    auto eigenvalues = getEigenvalues();

    if (eigenvalues.size() < 3){
        return;
    }

    // 3
    if (equal(eigenvalues[0], eigenvalues[1]) && equal(eigenvalues[1], eigenvalues[2])) {
        // a
        // МАТРИЦА ДИАГОНАЛЬНА => К ШАГУ 4
    } else if (equal(eigenvalues[0], eigenvalues[1]) || equal(eigenvalues[1], eigenvalues[2]) || equal(eigenvalues[0], eigenvalues[2])) {
        if (equal(eigenvalues[1], eigenvalues[2]))
            swap(eigenvalues[0], eigenvalues[2]);

        if (equal(eigenvalues[0], eigenvalues[2]))
            swap(eigenvalues[1], eigenvalues[2]);

        // в
    } else {
        // б
    }

}

vector<float> AbstractSurface::getEigenvalues() {
    Equation eigenequation(VF{

            equation.XX() * equation.YY() * equation.ZZ()
            + equation.XY() * equation.XZ() * equation.YZ() / 4.f
            - equation.XY() * equation.XY() * equation.ZZ() / 4.f
            - equation.YZ() * equation.YZ() * equation.XX() / 4.f
            - equation.XY() * equation.XY() * equation.ZZ() / 4.f,

            -equation.YY() * equation.ZZ()
            - equation.XX() * equation.YY()
            - equation.XX() * equation.ZZ()
            + equation.XZ() * equation.XZ() / 4.f
            + equation.YZ() * equation.YZ() / 4.f
            + equation.XY() * equation.XY() / 4.f,

            equation.XX()
            + equation.YY()
            + equation.ZZ(),

            -1
    });
    return eigenequation.solve();
}
