//
// Created by alexey on 30.01.2020.
//

#include <iostream>
#include "AbstractSurface.h"
#include "../Evaluations/Equation.h"
#include "../Evaluations/Calculations.cpp"
#include "../Evaluations/EquationSystem.h"

AbstractSurface::AbstractSurface(VF coef) : equation(coef), canonical(VF(10)), temporary(VF(10)) {}

AbstractSurface::AbstractSurface() : equation(VF(10)), canonical(VF(10)), temporary(VF(10)) {}

void AbstractSurface::getCanonical() {
    // mathhelpplanet.com/static.php?p=privedenie-uravneniya-poverhnosti-k-kanonicheskomu-vidu
    vector<VF> ST;
    VF eigenvalues;
    // 1
    if (is_zero(equation.XY()) &&
        is_zero(equation.XZ()) &&
        is_zero(equation.YZ())) {
        ST = {{1, 0, 0},
              {0, 1, 0},
              {0, 0, 1}};
        eigenvalues = {
                equation.XX(),
                equation.YY(),
                equation.ZZ()
        };
    } else {

        // 2
        eigenvalues = getEigenvalues();

        if (eigenvalues.size() < 3) {
            return;
        }

        for (auto value : eigenvalues) {
            cout << value << " ";
        }
        cout << endl;

        // 3
        if (equal(eigenvalues[0], eigenvalues[1]) && equal(eigenvalues[1], eigenvalues[2])) {
            // a
            ST = {{1, 0, 0},
                  {0, 1, 0},
                  {0, 0, 1}};
        } else if (equal(eigenvalues[0], eigenvalues[1]) || equal(eigenvalues[1], eigenvalues[2]) ||
                   equal(eigenvalues[0], eigenvalues[2])) {
            if (equal(eigenvalues[1], eigenvalues[2]))
                swap(eigenvalues[0], eigenvalues[2]);

            if (equal(eigenvalues[0], eigenvalues[2]))
                swap(eigenvalues[1], eigenvalues[2]);

            // eigenvalues[2] - простой

            auto eigenvalue = eigenvalues[2];

            EquationSystem es({
                                      {
                                              equation.XX() - eigenvalue,
                                              equation.XY(),
                                              equation.XZ(),
                                              0
                                      },

                                      {
                                              equation.XY(),
                                              equation.YY() - eigenvalue,
                                              equation.YZ(),
                                              0
                                      },
                                      {
                                              equation.XZ(),
                                              equation.YZ(),
                                              equation.ZZ() -
                                              eigenvalue,
                                              0
                                      },
                              });
            auto l3 = es.solve();

            VF l2;
            if (!is_zero(equation.XX() - eigenvalue) || !is_zero(equation.XY()) || !is_zero(equation.XZ())) {
                l2 = {equation.XX() - eigenvalue, equation.XY(), equation.XZ()};
            } else if (!is_zero(equation.XY()) || !is_zero(equation.YY() - eigenvalue) || !is_zero(equation.YZ())) {
                l2 = {equation.XY(), equation.YY() - eigenvalue, equation.YZ()};
            } else {
                l2 = {equation.XZ(), equation.YZ(), equation.ZZ() - eigenvalue};
            }
            // l1 = [l2 x l3]
            VF l1 = {
                    +(l2[1] * l3[2] - l3[1] * l2[2]),
                    -(l2[0] * l3[2] - l3[0] * l2[2]),
                    +(l2[0] * l3[1] - l3[0] * l2[1])
            };

            float d = 0;
            for (float l : l1) {
                d += l * l;
            }
            for (float &l : l1) {
                l /= sqrt(d);
            }

            d = 0;
            for (float l : l2) {
                d += l * l;
            }
            for (float &l : l2) {
                l /= sqrt(d);
            }

            d = 0;
            for (float l : l3) {
                d += l * l;
            }
            for (float &l : l3) {
                l /= sqrt(d);
            }


            ST = {l1, l2, l3};

        } else {
            // б
            for (float eigenvalue : eigenvalues) {
                EquationSystem es({
                                          {
                                                  equation.XX() - eigenvalue,
                                                  equation.XY() / 2,
                                                  equation.XZ() / 2,
                                                  0
                                          },

                                          {
                                                  equation.XY() / 2,
                                                  equation.YY() - eigenvalue,
                                                  equation.YZ() / 2,
                                                  0
                                          },
                                          {
                                                  equation.XZ() / 2,
                                                  equation.YZ() / 2,
                                                  equation.ZZ() - eigenvalue,
                                                  0
                                          },
                                  });
                auto solved = es.solve();
                float d = 0;
                for (float solve : solved) {
                    d += solve * solve;
                }
                for (float &solve : solved) {
                    solve /= sqrt(d);
                }
                ST.push_back(solved);
            }
        }
    }

    // 4
    temporary.X() = ST[0][0] * equation.X() + ST[0][1] * equation.Y() + ST[0][2] * equation.Z();
    temporary.Y() = ST[1][0] * equation.X() + ST[1][1] * equation.Y() + ST[1][2] * equation.Z();
    temporary.Z() = ST[2][0] * equation.X() + ST[2][1] * equation.Y() + ST[2][2] * equation.Z();

//    temporary.X() *= 2;
//    temporary.Y() *= 2;
//    temporary.Z() *= 2;

    temporary.XX() = eigenvalues[0];
    temporary.YY() = eigenvalues[1];
    temporary.ZZ() = eigenvalues[2];

    temporary.D() = equation.D();

    temporary.print();


    if (is_zero(temporary.X()) && is_zero(temporary.Y()) && is_zero(temporary.Z())) {
        // a TODO: => 5
    } else {
        if (!is_zero(temporary.X()) && !is_zero(temporary.XX())) {
            // x^2 => (x + X/2XX)^2
            // D => D - X*X/4XX
            temporary.D() -= temporary.X() * temporary.X() / (4 * temporary.XX());
            temporary.X() = 0;
        }
        if (!is_zero(temporary.Y()) && !is_zero(temporary.YY())) {
            // y^2 => (y + Y/2YY)^2
            // D => D - Y*Y/4YY
            temporary.D() -= temporary.Y() * temporary.Y() / (4 * temporary.YY());
            temporary.Y() = 0;
        }
        if (!is_zero(temporary.Z()) && !is_zero(temporary.ZZ())) {
            // z^2 => (z + Z/2ZZ)^2
            // D => D - Z*Z/4ZZ
            temporary.D() -= temporary.Z() * temporary.Z() / (4 * temporary.ZZ());
            temporary.Z() = 0;
        }

        if (!is_zero(temporary.Y()) && !is_zero(temporary.Z()) &&
            is_zero(temporary.YY()) && is_zero(temporary.ZZ())) {
            // y => (Y / 2 * y + Z / 2 * z + D / 2) * 2 sqrt(Y * Y / 4 + Z * Z / 4)
            // z => (- Z / 2 * y + Y / 2 * z) * 2 sqrt(Y * Y / 4 + Z * Z / 4)
            temporary.Y() = 2 * sqrt(temporary.Y() * temporary.Y() / 4 + temporary.Z() * temporary.Z() / 4);
            temporary.Z() = 0;
        }
        // TODO: X & Y, X & Z

        if (!is_zero(temporary.X()) && !is_zero(temporary.D())) {
            // x => (x + D)
            // D => 0
            temporary.D() = 0;
        }

        if (!is_zero(temporary.Y()) && !is_zero(temporary.D())) {
            // y => (y + D)
            // D => 0
            temporary.D() = 0;
        }

        if (!is_zero(temporary.Z()) && !is_zero(temporary.D())) {
            // z => (z + D)
            // D => 0
            temporary.D() = 0;
        }

    }

    temporary.print();


}

vector<float> AbstractSurface::getEigenvalues() {
    VF eq_coef = {

            equation.XX() * equation.YY() * equation.ZZ()
            + equation.XY() * equation.XZ() * equation.YZ() / 4.f
            - equation.XY() * equation.XY() * equation.ZZ() / 4.f
            - equation.YZ() * equation.YZ() * equation.XX() / 4.f
            - equation.XZ() * equation.XZ() * equation.YY() / 4.f,

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
    };

    for (auto coef : eq_coef) {
        cout << coef << " ";
    }
    cout << endl;

    Equation eigenequation(eq_coef);
    return eigenequation.solve();
}
