//
// Created by alexey on 30.01.2020.
//

#include "AbstractSurface.h"
#include "../Evaluations/Equation.h"
#include "../Evaluations/Calculations.cpp"
#include "../Evaluations/EquationSystem.h"

AbstractSurface::AbstractSurface(VF coef) : equation(coef), canonical(VF(10)), temporary(VF(10)) {}

AbstractSurface::AbstractSurface() : equation(VF(10)), canonical(VF(10)), temporary(VF(10)) {}

void AbstractSurface::getCanonical() {
    // mathhelpplanet.com/static.php?p=privedenie-uravneniya-poverhnosti-k-kanonicheskomu-vidu
    vector<VF> ST;
    // 1
    if (is_zero(equation.XY()) &&
        is_zero(equation.XZ()) &&
        is_zero(equation.YZ())) {
        ST = {{1, 0, 0},
              {0, 1, 0},
              {0, 0, 1}};
    } else {

        // 2
        auto eigenvalues = getEigenvalues();

        if (eigenvalues.size() < 3) {
            return;
        }

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
            for (float & l : l1) {
                l /= sqrt(d);
            }

            d = 0;
            for (float l : l2) {
                d += l * l;
            }
            for (float & l : l2) {
                l /= sqrt(d);
            }

            d = 0;
            for (float l : l3) {
                d += l * l;
            }
            for (float & l : l3) {
                l /= sqrt(d);
            }


            ST = {l1, l2, l3};

        } else {
            // б
            for (float eigenvalue : eigenvalues) {
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
    temporary.X() = ST[0][0] * equation.X() + ST[0][1].

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
