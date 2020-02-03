#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "bits/stdc++.h"
#include "Primary/Point.h"
#include "Surfaces/Ellipsoid.h"
#include "Surfaces/Paraboloid.h"
#include "Evaluations/Equation.h"
#include "Evaluations/EquationSystem.h"
#include "Evaluations/Calculations.cpp"
#include "SurfaceEquations/SurfaceEquation.h"

typedef tuple<float, float, float, float> tuple4f;
typedef vector<Point> VP;
typedef vector<float> VF;

bool equal_surfaces = false;

using namespace std;

GLfloat a = -90.f, b = 0.f, c = -90.f;
//GLfloat a = 0.f, b = 0.f, c = 0.f;
GLfloat x = 0.f, y = 0.f, z = 0.f;

bool transp = false;
bool hide = false;
bool typefl = true;

vector<float> first_coefs, second_coefs;

SurfaceEquation *first_equation, *second_equation;

AbstractSurface *first_surface, *second_surface;



//GLint radius = 100;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (key == GLFW_KEY_ENTER)
            typefl = !typefl;

        if (key == GLFW_KEY_SPACE || key == GLFW_KEY_H)
            hide = !hide;

        if (key == GLFW_KEY_BACKSLASH)
            transp = !transp;
    }

    if (action = GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) {
            a -= 3.f;
        }
        if (key == GLFW_KEY_DOWN) {
            a += 3.f;
        }

        if (key == GLFW_KEY_KP_1) {
            b += 3.f;
        }
        if (key == GLFW_KEY_SLASH) {
            b -= 3.f;
        }

        if (key == GLFW_KEY_RIGHT) {
            c -= 3.f;
        }
        if (key == GLFW_KEY_LEFT) {
            c += 3.f;
        }

        //////////////

        if (key == GLFW_KEY_A) {
            x -= 3.f;
        }
        if (key == GLFW_KEY_D) {
            x += 3.f;
        }

        if (key == GLFW_KEY_S) {
            y -= 3.f;
        }
        if (key == GLFW_KEY_W) {
            y += 3.f;
        }

        if (key == GLFW_KEY_Z) {
            z -= 3.f;
        }
        if (key == GLFW_KEY_Q) {
            z += 3.f;
        }

    }
}

//GLfloat project[] = {
//        1.f, 0.f, 0.f, 0.5f,
//        0.f, 1.f, 0.f, 0.f,
//        0.f, 0.f, 1.f, 0.5f,
//        0.f, 0.f, 0.f, 1.f };

void drawSurface(const vector<Point> &Vertices, tuple4f color, float move) {

    auto[R, G, B, A] = color;

    int type = (typefl ? GL_LINE_STRIP : GL_QUADS
    );

    if (Vertices.size() < 4)
        type = GL_POINTS;
    glColor4f(R, G, B, 1);

    if (transp) {
        glColor4f(R, G, B, A);
        if (!typefl)
            type = GL_QUADS;
    }
    glPointSize(5);
    glLineWidth(1);

    if (!hide) {
        glBegin(type);
        for (auto p : Vertices) {
            glVertex3f(p.x + x, p.y + y + move, p.z + z + move);
        }
        glEnd();
    }
}

vector<Point> make_intersect() {
//    VF ellipsoid = {1. / (100 * 100), 1. / (150 * 150), 1. / (100 * 100)};
//    VF paraboloid = {1. / (10 * 10), 1. / (10 * 10), 0.};

    vector<Point> intersect;

    auto first_quadric = first_equation->get_canonical();
    auto second_quadric = second_equation->get_equation();

    float intersect_epsilon = 1, edge_epsilon = 5;

    float max_z = 101;
    float min_z = -101;

//    glColor3f(0, 0, 1);
//    glBegin(GL_POINTS);

    for (float i = max_z; i >= min_z; i -= 0.3) {
//        float rx = 1 - float(i * i) * (ellipsoid[2]);
        float rx = -first_quadric.D() - i * i * first_quadric.ZZ() - i * first_quadric.Z();
        VF t = VF{first_quadric.XX() / rx, first_quadric.YY() / rx};

        for (float j = sqrt(1 / t[1]) + 1; j >= sqrt(1 / t[1]) - 1; j -= 0.1) {
            if (less_zero(float(j * j) * (t[1]) - 1) || is_zero(float(j * j) * (t[1]) - 1)) {
                float ry = 1 - float(j * j) * t[1];
                ry = (ry > 0 ? ry : 0);
                float y = j, z = i;
                float first_x = sqrt(ry) / sqrt(t[0]);
                bool correct = false, less_correct = false;
                VF eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                  y * z * second_quadric.YZ()
                                  + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                  second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                  second_quadric.XX(), 0})
                        .solve();
                for (float second_x : eq) {
                    if (equal_eps(second_x, first_x, edge_epsilon))
                        correct = true;
                    if (equal_eps(second_x, -first_x, edge_epsilon))
                        less_correct = true;
                }

                if (!correct || !less_correct) {
                    z++;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, edge_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, edge_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    z -= 2;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, edge_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, edge_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    z++;
                    y++;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, edge_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, edge_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    y -= 2;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, edge_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, edge_epsilon))
                            less_correct = true;
                    }
                }

                if (correct) {
                    intersect.push_back(Point(first_x, j, i));
                }
                if (less_correct) {
                    intersect.push_back(Point(-first_x, j, i));
                }
            }
        }
    }

    for (int i = sqrt(1 / first_quadric.ZZ()); i >= -sqrt(1 / first_quadric.ZZ()); i -= 1) {
        float rx = -first_quadric.D() - float(i * i) * first_quadric.ZZ() - i * first_quadric.Z();
        VF t = VF{first_quadric.XX() / rx, first_quadric.YY() / rx};

        for (int j = sqrt(1 / t[1]); j >= -sqrt(1 / t[1]); j -= 1) {
            if (1 >= float(j * j) * (t[1])) {
                float ry = 1 - float(j * j) * t[1];
                float y = j, z = i;
                float first_x = sqrt(ry) / sqrt(t[0]);
                bool correct = false, less_correct = false;
                VF eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                  y * z * second_quadric.YZ()
                                  + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                  second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                  second_quadric.XX(), 0})
                        .solve();
                for (float second_x : eq) {
                    //cout << "SOLVED: " << second_x << " " << first_x << endl;
                    if (equal_eps(second_x, first_x, intersect_epsilon))
                        correct = true;
                    if (equal_eps(second_x, -first_x, intersect_epsilon))
                        less_correct = true;
                }

                if (!correct || !less_correct) {
                    z++;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, intersect_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, intersect_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    z -= 2;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, intersect_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, intersect_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    z++;
                    y++;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, intersect_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, intersect_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    y -= 2;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, intersect_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, intersect_epsilon))
                            less_correct = true;
                    }
                }

                if (correct) {
                    intersect.emplace_back(first_x, j, i);
                }
                if (less_correct) {
                    intersect.emplace_back(-first_x, j, i);
                }
            }
        }
    }

    for (int i = max_z; i >= min_z; i -= 1) {
        float rx = -first_quadric.D() - i * i * first_quadric.ZZ() - i * first_quadric.Z();
        VF t = VF{first_quadric.XX() / rx, first_quadric.YY() / rx};

        for (float j = -sqrt(1 / t[1]) + 2; j >= -sqrt(1 / t[1]) - 2; j -= 0.1) {
            if (1 >= float(j * j) * (t[1])) {
                float ry = 1 - float(j * j) * t[1];
                float y = j, z = i;
                float first_x = sqrt(ry) / sqrt(t[0]);
                bool correct = false, less_correct = false;
                VF eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z()
                                  + y * z * second_quadric.YZ()
                                  + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                  second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                  second_quadric.XX(), 0})
                        .solve();
                for (float second_x : eq) {
                    if (equal_eps(second_x, first_x, edge_epsilon))
                        correct = true;
                    if (equal_eps(second_x, -first_x, edge_epsilon))
                        less_correct = true;
                }

                if (!correct || !less_correct) {
                    z++;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, edge_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, edge_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    z -= 2;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, edge_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, edge_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    z++;
                    y++;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, edge_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, edge_epsilon))
                            less_correct = true;
                    }
                }

                if (!correct || !less_correct) {
                    y -= 2;
                    eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                   y * z * second_quadric.YZ()
                                   + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                   second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                   second_quadric.XX(), 0})
                            .solve();
                    for (float second_x : eq) {
                        if (equal_eps(second_x, first_x, edge_epsilon))
                            correct = true;
                        if (equal_eps(second_x, -first_x, edge_epsilon))
                            less_correct = true;
                    }
                }

                if (correct) {
                    intersect.push_back(Point(first_x, j, i));
                }
                if (less_correct) {
                    intersect.push_back(Point(-first_x, j, i));
                }
            }
        }
    }
//    glEnd();
    return intersect;
}

void draw_intersect(VP intersect) {
    glColor4f(0, 0, 1, 1);
    glPointSize(3);
    glBegin(GL_POINTS);
    for (Point p : intersect) {
        //cout << p.x << " " << p.y << " " << p.z << endl;
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}


GLFWwindow *initWindow(const int resX, const int resY) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(resX, resY, "InterSect", NULL, NULL);

    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);  // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND); //Enable blending.
    glEnable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

//    GLfloat lightpos[] = {0., 0., 1., 0.};
//    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    return window;
}


//void display(GLFWwindow *window, vector<float> first_params, vector<float> second_params) {
void display(GLFWwindow *window) {

    vector<Point> vertices_first = first_surface->makeMash();

    vector<Point> vertices_second = second_surface->makeMash();

    vector<Point> intersect = make_intersect();

//    vector<Point> vertices_first = makeEllipsoid(100, 150, 100);
//    vector<Point> vertices_second = makeParaboloid(10, 10);

//    glRotatef(-90.f, 1, 0, 0);
//    glRotatef(90.f, 0, 0, 1);

    while (!glfwWindowShouldClose(window)) {
        GLint windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        //glClearColor(0.7, 1, 1, 1.0);
        glClearColor(0, 0, 0, 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        //glMultMatrixf(project);

        glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2,
                windowHeight / 2, -(windowWidth + windowHeight) / 4, (windowWidth + windowHeight) / 4);

        glMatrixMode(GL_MODELVIEW_MATRIX);

        //drawCube(window);

//        glTranslatef(x, y, z);
//
        glRotatef(a, 1, 0, 0);
        glRotatef(b, 0, 1, 0);
        glRotatef(c, 0, 0, 1);
//
//        glTranslatef(-x, -y, -z);

        if (!equal_surfaces) {
            glPushMatrix();

            //make_intersect();

            draw_intersect(intersect);

            glPushMatrix();
            float mulm[] = {1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1};

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    mulm[4 * i + j] = first_equation->get_mul_matrix()[i][j];
                }
                mulm[12 + i] = first_equation->get_additional_vector()[i];
            }

            glMultMatrixf(mulm);


            //glScaled(1, 5, 1);

            drawSurface(vertices_first, {0, 1, 0, 0.3}, 0);

            glPopMatrix();
        }

        float mulm2[] = {1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1};
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                mulm2[4 * i + j] = second_equation->get_mul_matrix()[i][j];
            }
            mulm2[12 + i] = second_equation->get_additional_vector()[i];
        }
        glMultMatrixf(mulm2);

        tuple4f second_color = (equal_surfaces ? tuple4f{0, 0, 1, 1} : tuple4f{1, 0, 0, 0.3});

        drawSurface(vertices_second, second_color, 0);

        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();

        glPushMatrix();
    }
}

string read_file(const string &path) {
    ifstream source_stream(path);
    ostringstream string_stream;

    string_stream << source_stream.rdbuf();
    source_stream.close();

    return string_stream.str();
}

inline bool space(char c) {
    return isspace(c);
}

inline bool notspace(char c) {
    return !isspace(c);
}

vector<string> split(const string &s) {
    typedef string::const_iterator iter;
    vector<string> ret;
    iter i = s.begin();
    while (i != s.end()) {
        i = find_if(i, s.end(), notspace);
        iter j = find_if(i, s.end(), space);
        if (i != s.end()) {
            ret.push_back(string(i, j));
            i = j;
        }
    }
    return ret;
}

bool read_equations() {
    string input_file = "/home/alexey/CLionProjects/InterSect/config.txt";
    string source = read_file(input_file);
    vector<string> params = split(source);
    if (params.size() < 30)
        return false;
    vector<string> first_params(params.begin() + 10, params.begin() + 20);
    vector<string> second_params(params.begin() + 20, params.begin() + 30);
    for (int i = 0; i < first_params.size(); ++i) {
        cout << i << " " << first_params[i] << endl;
        first_coefs.push_back(stof(first_params[i]));
        cout << i << " " << first_coefs[i] << endl;
    }
    for (int i = 0; i < second_params.size(); ++i) {
        cout << i << " " << second_params[i] << endl;
        second_coefs.push_back(stof(second_params[i]));
        cout << i << " " << second_coefs[i] << endl;
    }
    return true;
}

AbstractSurface *make_surface(SurfaceEquation &se) {
    if (se.get_type() == ELLIPSOID) {
        cout << "create Ellipsoid: " << endl;
        cout << se.get_canonical().XX() << 1 / se.get_canonical().XX() << endl;
        return new Ellipsoid(sqrt(1 / se.get_canonical().XX()),
                             sqrt(1 / se.get_canonical().YY()),
                             sqrt(1 / se.get_canonical().ZZ()));
    } else if (se.get_type() == PARABOLOID_ELLIPTIC) {
        return new Paraboloid(sqrt(1 / se.get_canonical().XX()),
                              sqrt(1 / se.get_canonical().YY()));
    }
    return nullptr;
}

int main(int argc, char **argv) {

    read_equations();

    float coef = 0;
    bool equals_temp = true;
    for (int i = 0; i < 10; ++i) {
        if (!is_zero(first_coefs[i]) && !is_zero(second_coefs[i])) {
            if (is_zero(coef)) {
                coef = first_coefs[i] / second_coefs[i];
            } else {
                equals_temp = equals_temp && (equal(coef, first_coefs[i] / second_coefs[i]));
            }
        }
    }

    equal_surfaces = equals_temp;

    first_equation = new SurfaceEquation(first_coefs);
    second_equation = new SurfaceEquation(second_coefs);

    first_equation->canonizate();
    cout << "FIRST TYPE: " << first_equation->get_type() << endl;
    first_surface = make_surface(*first_equation);

    second_equation->canonizate();
    cout << "SECOND TYPE: " << second_equation->get_type() << endl;
    second_surface = make_surface(*second_equation);


    GLFWwindow *window = initWindow(800, 600);
    if (nullptr != window) {
        display(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    delete first_surface;
    delete second_surface;

    delete first_equation;
    delete second_equation;






//    SurfaceEquation as(VF{1, 1, 0, 0, 0, 0, 2, -4, 2, 1});
//// quite OK
//
////    SurfaceEquation as(VF{1, 0, 0, 0, 0, 0, 0, 6, -8, 10});
//// quite OK
//
////    SurfaceEquation as(VF{3, -7, 3, 8, -8, -8, 10, -14, -6, -8});
//
//    as.canonizate();
//
//    cout << as.get_type() << endl;

    return 0;
}

//EquationSystem eq({
////                              {2, 2, -1, 0},
////                              {5, 4, -6, 0},
////                              {3, 2, -5, 0},
//                              { 1, 1, 1, 0 },
//                              { 1, 1, 1, 0 },
//                              { 1, 1, 1, 0 }
//                      });
//
//    auto v = eq.solve();
//
//    for (auto el : v) {
//        cout << el << endl;
//    }