#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "bits/stdc++.h"
#include "Primary/Point.h"
#include "Surfaces/Ellipsoid.h"
#include "Surfaces/ParaboloidElliptic.h"
#include "Evaluations/Equation.h"
#include "Evaluations/EquationSystem.h"
#include "Evaluations/Calculations.cpp"
#include "SurfaceEquations/SurfaceEquation.h"
#include "Surfaces/HyperboloidOneSheet.h"
#include "Surfaces/HyperboloidTwoSheet.h"
#include "Evaluations/Matrix.h"
#include "Surfaces/ParaboloidHyperbolic.h"

typedef tuple<float, float, float, float> tuple4f;
typedef vector<Point> VP;
typedef vector<float> VF;

bool equal_surfaces = false;

using namespace std;

GLfloat a = -90.f, b = 0.f, c = -90.f;
//GLfloat a = 0.f, b = 0.f, c = 0.f;
GLfloat x = 0.f, y = 0.f, z = 0.f;

float alpha = a, betta = b, gama = c;

bool transp = false;
bool hide = false;
bool typefl = true;
bool orts = true;

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

        if (key == GLFW_KEY_BACKSPACE)
            orts = !orts;
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

//        //////////////
//
//        if (key == GLFW_KEY_A) {
//            x -= 3.f;
//        }
//        if (key == GLFW_KEY_D) {
//            x += 3.f;
//        }
//
//        if (key == GLFW_KEY_S) {
//            y -= 3.f;
//        }
//        if (key == GLFW_KEY_W) {
//            y += 3.f;
//        }
//
//        if (key == GLFW_KEY_Z) {
//            z -= 3.f;
//        }
//        if (key == GLFW_KEY_Q) {
//            z += 3.f;
//        }

    }
}

GLfloat project[] = {
        1.f, 0.f, 0.f, 0.5f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.5f,
        0.f, 0.f, 0.f, 1.f};

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
        if (type == GL_LINE_STRIP) {
            for (int i = 0; i < Vertices.size() / 4; i++) {
                glBegin(GL_LINE_LOOP);
                for (int j = 0; j < 4; j++) {
                    Point p = Vertices[4 * i + j];
                    glVertex3f(p.x + x, p.y + y, p.z + z);
                }
                glEnd();
            }
        } else {
//            for (int i = 0; i < Vertices.size_x() / 4; i++) {
//                glBegin(GL_TRIANGLES);
//                Point p0 = Vertices[4 * i];
//                Point p1 = Vertices[4 * i + 1];
//                Point p2 = Vertices[4 * i + 2];
//                Point p3 = Vertices[4 * i + 3];
//                glVertex3f(p0.x + x, p0.y + y, p0.z + z);
//                glVertex3f(p1.x + x, p1.y + y, p1.z + z);
//                glVertex3f(p2.x + x, p2.y + y, p2.z + z);
//
//
//                glVertex3f(p0.x + x, p0.y + y, p0.z + z);
//                glVertex3f(p2.x + x, p2.y + y, p2.z + z);
//                glVertex3f(p3.x + x, p3.y + y, p3.z + z);
//                glEnd();
//            }

            glBegin(GL_QUADS);
            for (auto p : Vertices) {
                glVertex3f(p.x + x, p.y + y, p.z + z);
            }
            glEnd();
        }
    }
}

vector<Point> make_intersect() {

    vector<Point> intersect;

    auto first_quadric = first_equation->get_canonical();
    auto second_quadric = second_equation->get_equation();

    vector<VF> mul_reversed = Matrix(first_equation->get_mul_matrix()).reverse();
    VF additional = first_equation->get_additional_vector();

    float intersect_epsilon = 2, edge_epsilon = 2;

    float max_z = 200;
    float min_z = -200;

    if (first_equation->get_type() == ELLIPSOID) {
        max_z = 1 / sqrt(first_quadric.ZZ());
        min_z = -max_z;
    }

    bool test = false;

//    glColor3f(0, 0, 1);
//    glBegin(GL_POINTS);
//
    for (float i = max_z; i >= min_z; i -= 0.1) {
//        float rx = 1 - float(i * i) * (ellipsoid[2]);
        float rx = -first_quadric.D() - i * i * first_quadric.ZZ() - i * first_quadric.Z();
        VF t = VF{first_quadric.XX() / rx, first_quadric.YY() / rx};

        for (float j = sqrt(1 / t[1]) + 1; j >= sqrt(1 / t[1]) - 1; j -= 0.01) {
            if (less_zero(float(j * j) * (t[1]) - 1) || is_zero(float(j * j) * (t[1]) - 1)) {
                float ry = 1 - float(j * j) * t[1];
                float first_x = sqrt(ry) / sqrt(t[0]);

                if (test) {
                    intersect.emplace_back(first_x, j, i);
                    intersect.emplace_back(-first_x, j, i);
                    continue;
                }

                float y_less, y_great, z_less, z_great, x_less = -first_x, x_great = first_x;
                bool correct = false, correct_less = false;

                x_great = mul_reversed[0][0] * (first_x) + additional[0]
                          + mul_reversed[1][0] * (j)
                          + mul_reversed[2][0] * (i);

                y_great = mul_reversed[0][1] * (first_x)
                          + mul_reversed[1][1] * (j) + additional[1]
                          + mul_reversed[2][1] * (i);

                z_great = mul_reversed[2][0] * (first_x)
                          + mul_reversed[2][1] * (j)
                          + mul_reversed[2][2] * (i) + additional[2];

                float y = y_great;
                float z = z_great;


                VF eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                  y * z * second_quadric.YZ()
                                  + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                  second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                  second_quadric.XX(), 0})
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_great, intersect_epsilon))
                        correct = true;
                }

                x_less = mul_reversed[0][0] * (-first_x) + additional[0]
                         + mul_reversed[1][0] * (j)
                         + mul_reversed[2][0] * (i);

                y_less = mul_reversed[0][1] * (-first_x)
                         + mul_reversed[1][1] * (j) + additional[1]
                         + mul_reversed[2][1] * (i);

                z_less = mul_reversed[2][0] * (-first_x)
                         + mul_reversed[2][1] * (j)
                         + mul_reversed[2][2] * (i) + additional[2];

                y = y_less;
                z = z_less;

                eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                               y * z * second_quadric.YZ()
                               + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                               second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                               second_quadric.XX(), 0})
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_less, edge_epsilon))
                        correct_less = true;
                }

                if (correct) {
                    intersect.emplace_back(first_x, j, i);
                }
                if (correct_less) {
                    intersect.emplace_back(-first_x, j, i);
                }
            }
        }
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

    for (float i = max_z; i >= min_z; i -= 0.5) {
        float rx = -first_quadric.D() - float(i * i) * first_quadric.ZZ() - i * first_quadric.Z();
        VF t = VF{first_quadric.XX() / rx, first_quadric.YY() / rx};

        for (float j = 200; j >= -200; j -= 0.1) {
            if (1 >= float(j * j) * (t[1])) {
                float ry = 1 - float(j * j) * t[1];
                float first_x = sqrt(ry) / sqrt(t[0]);

                if (test) {
                    intersect.emplace_back(first_x, j, i);
                    intersect.emplace_back(-first_x, j, i);
                    continue;
                }

                float y_less, y_great, z_less, z_great, x_less = -first_x, x_great = first_x;
                bool correct = false, correct_less = false;

                x_great = mul_reversed[0][0] * (first_x) + additional[0]
                          + mul_reversed[1][0] * (j)
                          + mul_reversed[2][0] * (i);

                y_great = mul_reversed[0][1] * (first_x)
                          + mul_reversed[1][1] * (j) + additional[1]
                          + mul_reversed[2][1] * (i);

                z_great = mul_reversed[2][0] * (first_x)
                          + mul_reversed[2][1] * (j)
                          + mul_reversed[2][2] * (i) + additional[2];

                float y = y_great;
                float z = z_great;


                VF eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                  y * z * second_quadric.YZ()
                                  + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                  second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                  second_quadric.XX(), 0})
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_great, intersect_epsilon))
                        correct = true;
                }

                x_less = mul_reversed[0][0] * (-first_x) + additional[0]
                         + mul_reversed[1][0] * (j)
                         + mul_reversed[2][0] * (i);

                y_less = mul_reversed[0][1] * (-first_x)
                         + mul_reversed[1][1] * (j) + additional[1]
                         + mul_reversed[2][1] * (i);

                z_less = mul_reversed[2][0] * (-first_x)
                         + mul_reversed[2][1] * (j)
                         + mul_reversed[2][2] * (i) + additional[2];

                y = y_less;
                z = z_less;

                eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                               y * z * second_quadric.YZ()
                               + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                               second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                               second_quadric.XX(), 0})
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_less, intersect_epsilon))
                        correct_less = true;
                }

                if (correct) {
                    intersect.emplace_back(first_x, j, i);
                }
                if (correct_less) {
                    intersect.emplace_back(-first_x, j, i);
                }
            }
        }
    }
//
    for (float i = max_z; i >= min_z; i -= 0.1) {
        float rx = -first_quadric.D() - i * i * first_quadric.ZZ() - i * first_quadric.Z();
        VF t = VF{first_quadric.XX() / rx, first_quadric.YY() / rx};

        for (float j = -sqrt(1 / t[1]) + 1; j >= -sqrt(1 / t[1]) - 1; j -= 0.01) {
            if (1 >= float(j * j) * (t[1])) {
                float ry = 1 - float(j * j) * t[1];
                float first_x = sqrt(ry) / sqrt(t[0]);

                if (test) {
                    intersect.emplace_back(first_x, j, i);
                    intersect.emplace_back(-first_x, j, i);
                    continue;
                }

                float y_less, y_great, z_less, z_great, x_less = -first_x, x_great = first_x;
                bool correct = false, correct_less = false;

                x_great = mul_reversed[0][0] * (first_x) + additional[0]
                          + mul_reversed[1][0] * (j)
                          + mul_reversed[2][0] * (i);

                y_great = mul_reversed[0][1] * (first_x)
                          + mul_reversed[1][1] * (j) + additional[1]
                          + mul_reversed[2][1] * (i);

                z_great = mul_reversed[2][0] * (first_x)
                          + mul_reversed[2][1] * (j)
                          + mul_reversed[2][2] * (i) + additional[2];

                float y = y_great;
                float z = z_great;


                VF eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                                  y * z * second_quadric.YZ()
                                  + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                                  second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                                  second_quadric.XX(), 0})
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_great, intersect_epsilon))
                        correct = true;
                }

                x_less = mul_reversed[0][0] * (-first_x) + additional[0]
                         + mul_reversed[1][0] * (j)
                         + mul_reversed[2][0] * (i);

                y_less = mul_reversed[0][1] * (-first_x)
                         + mul_reversed[1][1] * (j) + additional[1]
                         + mul_reversed[2][1] * (i);

                z_less = mul_reversed[2][0] * (-first_x)
                         + mul_reversed[2][1] * (j)
                         + mul_reversed[2][2] * (i) + additional[2];

                y = y_less;
                z = z_less;

                eq = Equation({second_quadric.D() + y * second_quadric.Y() + z * second_quadric.Z() +
                               y * z * second_quadric.YZ()
                               + y * y * second_quadric.YY() + z * z * second_quadric.ZZ(),
                               second_quadric.X() + z * second_quadric.XZ() + y * second_quadric.XY(),
                               second_quadric.XX(), 0})
                        .solve();

                for (float second_x : eq) {
                    if (equal_eps(second_x, x_less, edge_epsilon))
                        correct_less = true;
                }

                if (correct) {
                    intersect.emplace_back(first_x, j, i);
                }
                if (correct_less) {
                    intersect.emplace_back(-first_x, j, i);
                }
            }
        }
    }
//    glEnd();
    return intersect;
}

void draw_intersect(VP intersect) {
    glColor4f(0, 0, 1, 1);
    glPointSize(5);
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

#define ORT_LENGTH 300
#define ORT_WIDTH 3

void render_x_ort() {
    glColor4f(1, 0, 0, 1);
    glLineWidth(ORT_WIDTH);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(ORT_LENGTH, 0, 0);
    glVertex3f(ORT_LENGTH - 10, 10, 0);
    glVertex3f(ORT_LENGTH, 0, 0);
    glVertex3f(ORT_LENGTH - 10, -10, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(ORT_LENGTH + 20, 5, 5);
    glVertex3f(ORT_LENGTH + 20, -5, -5);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(ORT_LENGTH + 20, 5, -5);
    glVertex3f(ORT_LENGTH + 20, -5, 5);
    glEnd();
}


void render_y_ort() {
    glColor4f(0, 1, 0, 1);
    glLineWidth(ORT_WIDTH);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, ORT_LENGTH, 0);
    glVertex3f(10, ORT_LENGTH - 10, 0);
    glVertex3f(0, ORT_LENGTH, 0);
    glVertex3f(-10, ORT_LENGTH - 10, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(-5, ORT_LENGTH + 20, 7);
    glVertex3f(7, ORT_LENGTH + 20, -5);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(0, ORT_LENGTH + 20, 0);
    glVertex3f(7, ORT_LENGTH + 20, 7);
    glEnd();
}


void render_z_ort() {
    glColor4f(0, 0, 1, 1);
    glLineWidth(ORT_WIDTH);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, ORT_LENGTH);
    glVertex3f(0, 10, ORT_LENGTH - 10);
    glVertex3f(0, 0, ORT_LENGTH);
    glVertex3f(0, -10, ORT_LENGTH - 10);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(0, -5, ORT_LENGTH + 30);
    glVertex3f(0, 5, ORT_LENGTH + 30);
    glVertex3f(0, -5, ORT_LENGTH + 20);
    glVertex3f(0, 5, ORT_LENGTH + 20);
    glEnd();

}


//void display(GLFWwindow *window, vector<float> first_params, vector<float> second_params) {
void display(GLFWwindow *window) {

    vector<Point> vertices_first = first_surface->make_mash();

    vector<Point> vertices_second = second_surface->make_mash();

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
        glClearColor(1, 1, 1, 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        //glMultMatrixf(project);

        glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2,
                windowHeight / 2, -(windowWidth + windowHeight) / 4, (windowWidth + windowHeight) / 4);

        glMatrixMode(GL_MODELVIEW_MATRIX);

        //drawCube(window);

//        glTranslatef(x, y, z);


        glRotatef(a, 1, 0, 0);
        glRotatef(b, 0, 1, 0);
        glRotatef(c, 0, 0, 1);



        if (orts) {
            render_x_ort();
            render_y_ort();
            render_z_ort();
        }

//
//        glTranslatef(-x, -y, -z);

        if (!equal_surfaces) {
            glPushMatrix();

            glEnd();

            //make_intersect();

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


            draw_intersect(intersect);
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
            ret.emplace_back(i, j);
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
        return new Ellipsoid(sqrt(1 / se.get_canonical().XX()),
                             sqrt(1 / se.get_canonical().YY()),
                             sqrt(1 / se.get_canonical().ZZ()));
    } else if (se.get_type() == PARABOLOID_ELLIPTIC) {
        return new ParaboloidElliptic(sqrt(1 / se.get_canonical().XX()),
                                      sqrt(1 / se.get_canonical().YY()));
    } else if (se.get_type() == PARABOLOID_HYPERBOLIC) {
        return new ParaboloidHyperbolic(sqrt(1 / se.get_canonical().XX()),
                                        sqrt(-1 / se.get_canonical().YY()));
    } else if (se.get_type() == HYPERBOLOID_ONE_SHEET) {
        return new HyperboloidOneSheet(sqrt(1 / se.get_canonical().XX()),
                                       sqrt(1 / se.get_canonical().YY()),
                                       sqrt(-1 / se.get_canonical().ZZ()));
    } else if (se.get_type() == HYPERBOLOID_TWO_SHEET) {
        return new HyperboloidTwoSheet(sqrt(1 / se.get_canonical().XX()),
                                       sqrt(1 / se.get_canonical().YY()),
                                       sqrt(-1 / se.get_canonical().ZZ()));
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
        } else if (is_zero(first_coefs[i]) && is_zero(second_coefs[i])) {
            continue;
        } else {
            equals_temp = false;
            break;
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


    GLFWwindow *window = initWindow(800, 700);
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