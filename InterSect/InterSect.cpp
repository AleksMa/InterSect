#include "InterSect.h"

#include <utility>

using namespace std;


vector<Point> InterSect::create_intersect() {
    if (first_equation->get_type() == PARABOLOID_HYPERBOLIC) {
        return make_ph_intersect();
    } else if (first_equation->get_type() != UNKNOWN) {
        return make_intersect();
    }
    return vector<Point>();
}

vector<Point> InterSect::make_intersect() {

    vector<Point> intersect;

    auto first_quadric = first_equation->get_canonical();
    auto second_quadric = second_equation->get_equation();

    vector<vector<float>> mul_reversed = Matrix(first_equation->get_mul_matrix()).reverse();
    vector<float> additional = first_equation->get_additional_vector();

    float intersect_epsilon = 1, edge_epsilon = 2.5;

    float max_z = first_surface->max_z();
    float min_z = first_surface->min_z();

    bool test = false;

    float z_iter = max_z;

    while ( z_iter >= min_z) {
        float rx = -first_quadric.D() - z_iter * z_iter * first_quadric.ZZ() - z_iter * first_quadric.Z();
        VF t = VF{first_quadric.XX() / rx, first_quadric.YY() / rx};

        float max_y = sqrt(1 / t[1]);
        float min_y = -sqrt(1 / t[1]);

        float y_iter = max_y + 0.001;

        while (y_iter >= max_y - 2) {
            if (less_zero(float(y_iter * y_iter) * (t[1]) - 1) || is_zero(float(y_iter * y_iter) * (t[1]) - 1)) {
                float ry = 1 - float(y_iter * y_iter) * t[1];
                float first_x = sqrt(ry) / sqrt(t[0]);

                if (test) {
                    intersect.emplace_back(first_x, y_iter, z_iter);
                    intersect.emplace_back(-first_x, y_iter, z_iter);
                    continue;
                }

                float y_less, y_great, z_less, z_great, x_less = -first_x, x_great = first_x;
                bool correct = false, correct_less = false;

                x_great = mul_reversed[0][0] * (first_x) + additional[0]
                          + mul_reversed[1][0] * (y_iter)
                          + mul_reversed[2][0] * (z_iter);

                y_great = mul_reversed[0][1] * (first_x)
                          + mul_reversed[1][1] * (y_iter) + additional[1]
                          + mul_reversed[2][1] * (z_iter);

                z_great = mul_reversed[2][0] * (first_x)
                          + mul_reversed[2][1] * (y_iter)
                          + mul_reversed[2][2] * (z_iter) + additional[2];

                float y = y_great;
                float z = z_great;


                VF equation_coefs = VF{second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                       y * z * second_quadric.YZ()
                                       + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                       second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                       second_quadric.XX(), 0};
                VF eq = Equation(equation_coefs)
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_great, intersect_epsilon))
                        correct = true;
                }

                if (!correct) {
                    bool zeros = true;
                    for (float c : equation_coefs) {
                        zeros = zeros && is_zero_eps(c, y_step);
                    }
                    correct = zeros;
                }


                x_less = mul_reversed[0][0] * (-first_x) + additional[0]
                         + mul_reversed[1][0] * (y_iter)
                         + mul_reversed[2][0] * (z_iter);

                y_less = mul_reversed[0][1] * (-first_x)
                         + mul_reversed[1][1] * (y_iter) + additional[1]
                         + mul_reversed[2][1] * (z_iter);

                z_less = mul_reversed[2][0] * (-first_x)
                         + mul_reversed[2][1] * (y_iter)
                         + mul_reversed[2][2] * (z_iter) + additional[2];

                y = y_less;
                z = z_less;

                equation_coefs = VF{second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                    y * z * second_quadric.YZ()
                                    + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                    second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                    second_quadric.XX(), 0};

                eq = Equation(equation_coefs)
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_less, intersect_epsilon))
                        correct_less = true;
                }


                if (!correct_less) {
                    bool zeros = true;
                    for (float c : equation_coefs) {
                        zeros = zeros && is_zero_eps(c, y_step);
                    }
                    correct_less = zeros;
                }


                if (correct) {
                    intersect.emplace_back(first_x, y_iter, z_iter);
                }
                if (correct_less) {
                    intersect.emplace_back(-first_x, y_iter, z_iter);
                }
            }
            y_iter -= y_step_ext;
        }

        y_iter = min_y + 2;
        while (y_iter >= min_y - 0.001) {
            if (1 >= float(y_iter * y_iter) * (t[1])) {
                float ry = 1 - float(y_iter * y_iter) * t[1];
                float first_x = sqrt(ry) / sqrt(t[0]);

                if (test) {
                    intersect.emplace_back(first_x, y_iter, z_iter);
                    intersect.emplace_back(-first_x, y_iter, z_iter);
                    continue;
                }

                float y_less, y_great, z_less, z_great, x_less = -first_x, x_great = first_x;
                bool correct = false, correct_less = false;

                x_great = mul_reversed[0][0] * (first_x) + additional[0]
                          + mul_reversed[1][0] * (y_iter)
                          + mul_reversed[2][0] * (z_iter);

                y_great = mul_reversed[0][1] * (first_x)
                          + mul_reversed[1][1] * (y_iter) + additional[1]
                          + mul_reversed[2][1] * (z_iter);

                z_great = mul_reversed[2][0] * (first_x)
                          + mul_reversed[2][1] * (y_iter)
                          + mul_reversed[2][2] * (z_iter) + additional[2];

                float y = y_great;
                float z = z_great;


                VF equation_coefs = VF{second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                       y * z * second_quadric.YZ()
                                       + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                       second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                       second_quadric.XX(), 0};
                VF eq = Equation(equation_coefs)
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_great, intersect_epsilon))
                        correct = true;
                }

                if (!correct) {
                    bool zeros = true;
                    for (float c : equation_coefs) {
                        zeros = zeros && is_zero_eps(c, y_step);
                    }
                    correct = zeros;
                }


                x_less = mul_reversed[0][0] * (-first_x) + additional[0]
                         + mul_reversed[1][0] * (y_iter)
                         + mul_reversed[2][0] * (z_iter);

                y_less = mul_reversed[0][1] * (-first_x)
                         + mul_reversed[1][1] * (y_iter) + additional[1]
                         + mul_reversed[2][1] * (z_iter);

                z_less = mul_reversed[2][0] * (-first_x)
                         + mul_reversed[2][1] * (y_iter)
                         + mul_reversed[2][2] * (z_iter) + additional[2];

                y = y_less;
                z = z_less;

                equation_coefs = VF{second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                    y * z * second_quadric.YZ()
                                    + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                    second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                    second_quadric.XX(), 0};

                eq = Equation(equation_coefs)
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_less, intersect_epsilon))
                        correct_less = true;
                }


                if (!correct_less) {
                    bool zeros = true;
                    for (float c : equation_coefs) {
                        zeros = zeros && is_zero_eps(c, y_step);
                    }
                    correct_less = zeros;
                }


                if (correct) {
                    intersect.emplace_back(first_x, y_iter, z_iter);
                }
                if (correct_less) {
                    intersect.emplace_back(-first_x, y_iter, z_iter);
                }
            }
            y_iter -= y_step_ext;
        }
        z_iter -= z_step_ext;
    }

    for (int k = 0; k < additional.size(); ++k) {
        cout << additional[k] << " ";
    }
    cout << endl;

    for (int l = 0; l < 3; ++l) {
        for (int i = 0; i < 3; ++i) {
            cout << mul_reversed[l][i] << " ";
        }
        cout << endl;
    }

    z_iter = max_z;

    while (z_iter >= min_z) {
        float rx = -first_quadric.D() - float(z_iter * z_iter) * first_quadric.ZZ() - z_iter * first_quadric.Z();
        VF t = VF{first_quadric.XX() / rx, first_quadric.YY() / rx};

        float max_y = sqrt(1 / t[1]);
        float min_y = -sqrt(1 / t[1]);

        float y_iter = max_y;

        while (y_iter >= min_y) {
            if (1 >= float(y_iter * y_iter) * (t[1])) {
                float ry = 1 - float(y_iter * y_iter) * t[1];
                float first_x = sqrt(ry) / sqrt(t[0]);

                if (test) {
                    intersect.emplace_back(first_x, y_iter, z_iter);
                    intersect.emplace_back(-first_x, y_iter, z_iter);
                    continue;
                }

                float y_less, y_great, z_less, z_great, x_less = -first_x, x_great = first_x;
                bool correct = false, correct_less = false;

                x_great = mul_reversed[0][0] * (first_x) + additional[0]
                          + mul_reversed[1][0] * (y_iter)
                          + mul_reversed[2][0] * (z_iter);

                y_great = mul_reversed[0][1] * (first_x)
                          + mul_reversed[1][1] * (y_iter) + additional[1]
                          + mul_reversed[2][1] * (z_iter);

                z_great = mul_reversed[2][0] * (first_x)
                          + mul_reversed[2][1] * (y_iter)
                          + mul_reversed[2][2] * (z_iter) + additional[2];

                float y = y_great;
                float z = z_great;


                VF equation_coefs = VF{second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                       y * z * second_quadric.YZ()
                                       + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                       second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                       second_quadric.XX(), 0};
                VF eq = Equation(equation_coefs)
                        .solve();

//                cout << "COORD: " << x_great << " " << y << " " << z << endl;
//                for (float f : VF{second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
//                                  y * z * second_quadric.YZ()
//                                  + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
//                                  second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
//                                  second_quadric.XX(), 0}) {
//                    cout << f << " ";
//                }
//                cout << endl;

                for (float second_x : eq) {
                    //cout << second_x << " ";
                    if (equal_eps(second_x, x_great, intersect_epsilon))
                        correct = true;
                }
                //cout << endl;

                if (!correct) {
                    bool zeros = true;
                    for (float c : equation_coefs) {
                        zeros = zeros && is_zero_eps(c, y_step);
                    }
                    correct = zeros;
                }


                x_less = mul_reversed[0][0] * (-first_x) + additional[0]
                         + mul_reversed[1][0] * (y_iter)
                         + mul_reversed[2][0] * (z_iter);

                y_less = mul_reversed[0][1] * (-first_x)
                         + mul_reversed[1][1] * (y_iter) + additional[1]
                         + mul_reversed[2][1] * (z_iter);

                z_less = mul_reversed[2][0] * (-first_x)
                         + mul_reversed[2][1] * (y_iter)
                         + mul_reversed[2][2] * (z_iter) + additional[2];

                y = y_less;
                z = z_less;

                equation_coefs = VF{second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                    y * z * second_quadric.YZ()
                                    + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                    second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                    second_quadric.XX(), 0};

                eq = Equation(equation_coefs)
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_less, intersect_epsilon))
                        correct_less = true;
                }


                if (!correct_less) {
                    bool zeros = true;
                    for (float c : equation_coefs) {
                        zeros = zeros && is_zero_eps(c, y_step);
                    }
                    correct_less = zeros;
                }


                if (correct) {
                    intersect.emplace_back(first_x, y_iter, z_iter);
                }
                if (correct_less) {
                    intersect.emplace_back(-first_x, y_iter, z_iter);
                }
            }
            y_iter -= y_step;
        }
        z_iter -= z_step;
    }

    return intersect;
}


vector<Point> InterSect::make_ph_intersect() {

    vector<Point> intersect;

    auto first_quadric = first_equation->get_canonical();
    auto second_quadric = second_equation->get_equation();

    vector<VF> mul_reversed = Matrix(first_equation->get_mul_matrix()).reverse();
    VF additional = first_equation->get_additional_vector();

    float intersect_epsilon = 1, edge_epsilon = 2.5;

    float max_x = first_surface->max_z();
    float min_x = -max_x;

    float size_z = first_surface->min_z();
//    float size_z = 40;

    bool test = false;

    float x_iter = min_x;

    while (x_iter <= max_x) {

        VF t = VF{first_quadric.XX() * x_iter * x_iter, -first_quadric.YY()};

        float z_iter = t[0];
        float start = z_iter;

        while (z_iter >= -size_z + t[0] - max_x * max_x * first_quadric.XX()) {
            float ry = t[0] - z_iter;
            float first_y = sqrt(ry) / sqrt(t[1]);

            if (test) {
                intersect.emplace_back(first_y, z_iter, x_iter);
                intersect.emplace_back(-first_y, z_iter, x_iter);
                continue;
            }

            // x_iter - x
            // z_iter - z

            float x_less, x_great, z_less, z_great, val_less = -first_y, val_great = first_y;
            bool correct = false, correct_less = false;

            x_great = mul_reversed[0][0] * (x_iter) + additional[0]
                      + mul_reversed[1][0] * (first_y)
                      + mul_reversed[2][0] * (z_iter);

            val_great = mul_reversed[0][1] * (x_iter)
                        + mul_reversed[1][1] * (first_y) + additional[1]
                        + mul_reversed[2][1] * (z_iter);

            z_great = mul_reversed[2][0] * (x_iter)
                      + mul_reversed[2][1] * (first_y)
                      + mul_reversed[2][2] * (z_iter) + additional[2];

            float x = x_great;
            float z = z_great;

            VF equation_coefs = {second_quadric.D() + x * second_quadric.X() + z * second_quadric.Z() +
                                 x * z * second_quadric.XZ()
                                 + x * x * second_quadric.XX() + z * z * second_quadric.ZZ(),
                                 second_quadric.Y() + z * second_quadric.YZ() + x * second_quadric.XY(),
                                 second_quadric.YY(), 0};

            VF eq = Equation(equation_coefs)
                    .solve();

            for (float second_y : eq) {
                if (equal_eps(second_y, val_great, intersect_epsilon))
                    correct = true;
            }

            if (!correct) {
                bool zeros = true;
                for (float c : equation_coefs) {
                    zeros = zeros && is_zero_eps(c, y_step);
                }
                correct = zeros;
            }

            x_less = mul_reversed[0][0] * (x_iter) + additional[0]
                     + mul_reversed[1][0] * (-first_y)
                     + mul_reversed[2][0] * (z_iter);

            val_less = mul_reversed[0][1] * (x_iter)
                       + mul_reversed[1][1] * (-first_y) + additional[1]
                       + mul_reversed[2][1] * (z_iter);

            z_less = mul_reversed[2][0] * (x_iter)
                     + mul_reversed[2][1] * (-first_y)
                     + mul_reversed[2][2] * (z_iter) + additional[2];

            x = x_less;
            z = z_less;

            equation_coefs = {second_quadric.D() + x * second_quadric.X() + z * second_quadric.Z() +
                              x * z * second_quadric.XZ()
                              + x * x * second_quadric.XX() + z * z * second_quadric.ZZ(),
                              second_quadric.Y() + z * second_quadric.YZ() + x * second_quadric.XY(),
                              second_quadric.YY(), 0};

            eq = Equation(equation_coefs)
                    .solve();

            for (float second_y : eq) {
                if (equal_eps(second_y, val_less, edge_epsilon))
                    correct_less = true;
            }

            if (!correct_less) {
                bool zeros = true;
                for (float c : equation_coefs) {
                    zeros = zeros && is_zero_eps(c, y_step);
                }
                correct_less = zeros;
            }

            if (correct) {
                intersect.emplace_back(x_iter, first_y, z_iter);
            }
            if (correct_less) {
                intersect.emplace_back(x_iter, -first_y, z_iter);
            }
            if (start - z_iter < 1) {
                z_iter -= z_step_ext;
            } else {
                z_iter -= z_step;
            }
        }
        x_iter += z_step_ext;
    }

    for (int k = 0; k < additional.size(); ++k) {
        cout << additional[k] << " ";
    }
    cout << endl;

    for (int l = 0; l < 3; ++l) {
        for (int i = 0; i < 3; ++i) {
            cout << mul_reversed[l][i] << " ";
        }
        cout << endl;
    }

    return intersect;
}

InterSect::InterSect(shared_ptr<SurfaceEquation> first_e, shared_ptr <SurfaceEquation> second_equation,
                     shared_ptr<AbstractSurface> first_surface, shared_ptr <AbstractSurface> second_surface): first_equation(std::move(first_e)),
                                                                                                              second_equation(std::move(second_equation)),
                                                                                                              first_surface(std::move(first_surface)),
                                                                                                              second_surface(std::move(second_surface)) {}
