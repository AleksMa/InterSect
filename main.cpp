#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "bits/stdc++.h"
#include "Primary/Point.h"
#include "Surfaces/Ellipsoid.h"
#include "Surfaces/Paraboloid.h"
#include "Evaluations/Equation.h"
#include "Evaluations/EquationSystem.h"
#include "Surfaces/AbstractSurface.h"

typedef tuple<float, float, float> tuple3f;
typedef vector<Point> VP;
typedef vector<float> VF;

using namespace std;

GLfloat a = -90.f, b = 0.f, c = -90.f;
GLfloat x = 0.f, y = 0.f, z = 0.f;

bool transp = false;
bool hide = false;
bool typefl = true;

vector<float> first_surface;
vector<float> second_surface;



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

vector<Point> makeEllipsoid(float A, float B, float C) {
    Ellipsoid ellipsoid(A, B, C);
    return ellipsoid.makeEllipsoidMash();
}

vector<Point> makeParaboloid(float A, float B) {
    Paraboloid paraboloid(A, B);
    return paraboloid.makeMash();
}

void drawSurface(vector<Point> Vertices, tuple3f color, float move) {

    auto[R, G, B] = color;

    int type = (typefl ? GL_LINE_STRIP : GL_QUADS
    );

    if (Vertices.size() < 4)
        type = GL_POINTS;
    glColor4f(R, G, B, 1);

    if (transp) {
        glColor4f(R, G, B, 0.3);
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

void drawIntersect() {
    VF ellipsoid = VF{1. / (100 * 100), 1. / (150 * 150), 1. / (100 * 100)};
    VF paraboloid = VF{1. / (10 * 10), 1. / (10 * 10), 0.};

    float eps = 1;

    glColor3f(0, 0, 1);
    glBegin(GL_POINTS);

    for (float i = sqrt(1 / ellipsoid[2]) + 1; i >= -1; i -= 0.5) {
        float rx = 1 - float(i * i) * (ellipsoid[2]);
        VF t = VF{ellipsoid[0] / rx, ellipsoid[1] / rx};

        for (float j = sqrt(1 / t[1]) + 1; j >= sqrt(1 / t[1]) - 2; j -= 0.1) {
            if (1 >= float(j * j) * (t[1])) {
                float ry = 1 - float(j * j) * t[1];
                float y = j, z = i;
                float first_x = sqrt(ry) / sqrt(t[0]);
                float second_x = sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                float second_x_higher =
                        sqrt(float(z + 1) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                float second_x_below =
                        sqrt(float(z - 1) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                y++;
                float second_x_right =
                        sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                y -= 2;
                float second_x_left =
                        sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                if (abs(first_x - second_x) < eps || abs(first_x + second_x) < eps ||
                    abs(first_x - second_x_higher) < eps || abs(first_x + second_x_higher) < eps ||
                    abs(first_x - second_x_below) < eps || abs(first_x + second_x_below) < eps ||
                    abs(first_x - second_x_right) < eps || abs(first_x + second_x_right) < eps ||
                    abs(first_x - second_x_left) < eps || abs(first_x + second_x_left) < eps) {
                    glVertex3f(first_x, j, i);
                }
                if (abs(-first_x - second_x) < eps || abs(-first_x + second_x) < eps ||
                    abs(-first_x - second_x_higher) < eps || abs(-first_x + second_x_higher) < eps ||
                    abs(-first_x - second_x_below) < eps || abs(-first_x + second_x_below) < eps ||
                    abs(-first_x - second_x_right) < eps || abs(-first_x + second_x_right) < eps ||
                    abs(-first_x - second_x_left) < eps || abs(-first_x + second_x_left) < eps) {
                    glVertex3f(-first_x, j, i);
                }
            }
        }
    }

    for (int i = sqrt(1 / ellipsoid[2]) + 1; i >= -1; i -= 1) {
        float rx = 1 - float(i * i) * (ellipsoid[2]);
        VF t = VF{ellipsoid[0] / rx, ellipsoid[1] / rx};

        for (int j = sqrt(1 / t[1]); j >= -sqrt(1 / t[1]); j -= 1) {
            if (1 >= float(j * j) * (t[1])) {
                float ry = 1 - float(j * j) * t[1];
                float y = j, z = i;
                float first_x = sqrt(ry) / sqrt(t[0]);
                float second_x = sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                float second_x_higher =
                        sqrt(float(z + 1) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                float second_x_below =
                        sqrt(float(z - 1) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                y++;
                float second_x_right =
                        sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                y -= 2;
                float second_x_left =
                        sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                if (abs(first_x - second_x) < eps || abs(first_x + second_x) < eps ||
                    abs(first_x - second_x_higher) < eps || abs(first_x + second_x_higher) < eps ||
                    abs(first_x - second_x_below) < eps || abs(first_x + second_x_below) < eps ||
                    abs(first_x - second_x_right) < eps || abs(first_x + second_x_right) < eps ||
                    abs(first_x - second_x_left) < eps || abs(first_x + second_x_left) < eps) {
                    glVertex3f(first_x, j, i);
                }
                if (abs(-first_x - second_x) < eps || abs(-first_x + second_x) < eps ||
                    abs(-first_x - second_x_higher) < eps || abs(-first_x + second_x_higher) < eps ||
                    abs(-first_x - second_x_below) < eps || abs(-first_x + second_x_below) < eps ||
                    abs(-first_x - second_x_right) < eps || abs(-first_x + second_x_right) < eps ||
                    abs(-first_x - second_x_left) < eps || abs(-first_x + second_x_left) < eps) {
                    glVertex3f(-first_x, j, i);
                }
            }
        }
    }

    for (float i = sqrt(1 / ellipsoid[2]) + 1; i >= -1; i -= 0.5) {
        float rx = 1 - float(i * i) * (ellipsoid[2]);
        VF t = VF{ellipsoid[0] / rx, ellipsoid[1] / rx};

        for (float j = -sqrt(1 / t[1]) + 2; j >= -sqrt(1 / t[1]) - 1; j -= 0.1) {
            if (1 >= float(j * j) * (t[1])) {
                float ry = 1 - float(j * j) * t[1];
                float y = j, z = i;
                float first_x = sqrt(ry) / sqrt(t[0]);
                float second_x = sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                float second_x_higher =
                        sqrt(float(z + 1) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                float second_x_below =
                        sqrt(float(z - 1) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                y++;
                float second_x_right =
                        sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                y -= 2;
                float second_x_left =
                        sqrt(float(z) + 50 - paraboloid[1] * float((y + 50) * (y + 50))) / sqrt(paraboloid[0]);
                if (abs(first_x - second_x) < eps || abs(first_x + second_x) < eps ||
                    abs(first_x - second_x_higher) < eps || abs(first_x + second_x_higher) < eps ||
                    abs(first_x - second_x_below) < eps || abs(first_x + second_x_below) < eps ||
                    abs(first_x - second_x_right) < eps || abs(first_x + second_x_right) < eps ||
                    abs(first_x - second_x_left) < eps || abs(first_x + second_x_left) < eps) {
                    glVertex3f(first_x, j, i);
                }
                if (abs(-first_x - second_x) < eps || abs(-first_x + second_x) < eps ||
                    abs(-first_x - second_x_higher) < eps || abs(-first_x + second_x_higher) < eps ||
                    abs(-first_x - second_x_below) < eps || abs(-first_x + second_x_below) < eps ||
                    abs(-first_x - second_x_right) < eps || abs(-first_x + second_x_right) < eps ||
                    abs(-first_x - second_x_left) < eps || abs(-first_x + second_x_left) < eps) {
                    glVertex3f(-first_x, j, i);
                }
            }
        }
    }
    glEnd();
}


GLFWwindow *initWindow(const int resX, const int resY) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);

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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
    glEnable(GL_BLEND); //Enable blending.

//    GLfloat lightpos[] = {0., 0., 1., 0.};
//    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    return window;
}


//void display(GLFWwindow *window, vector<float> first_params, vector<float> second_params) {
void display(GLFWwindow *window) {

    vector<Point> vertices_first = makeEllipsoid(sqrt(1 / first_surface[0]),
                                                 sqrt(1 / first_surface[1]),
                                                 sqrt(1 / first_surface[2]));

    vector<Point> vertices_second = makeParaboloid(sqrt(1 / second_surface[0]),
                                                   sqrt(1 / second_surface[1]));

//    vector<Point> vertices_first = makeEllipsoid(100, 150, 100);
//    vector<Point> vertices_second = makeParaboloid(10, 10);

//    glRotatef(-90.f, 1, 0, 0);
//    glRotatef(90.f, 0, 0, 1);

    while (!glfwWindowShouldClose(window)) {
        GLint windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        //glClearColor(0.7, 1, 1, 1.0);
        glClearColor(0, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        //glMultMatrixf(project);

        glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2,
                windowHeight / 2, -(windowWidth + windowHeight) / 4, (windowWidth + windowHeight) / 4);

        glMatrixMode(GL_MODELVIEW_MATRIX);

        //drawCube(window);

        glTranslatef(x, y, z);

        glRotatef(a, 1, 0, 0);
        glRotatef(b, 0, 1, 0);
        glRotatef(c, 0, 0, 1);

        glTranslatef(-x, -y, -z);

        drawIntersect();

        drawSurface(vertices_first, {0, 1, 0}, 0);

        drawSurface(vertices_second, {1, 0, 0}, -50);

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


int main(int argc, char **argv) {
//    string input_file = "/home/alexey/CLionProjects/InterSect/config.txt";
//    string source = read_file(input_file);
//    vector<string> params = split(source);
//    if(params.size() < 30)
//        exit(1);
//    vector<string> first_params(params.begin() + 10, params.begin() + 20);
//    vector<string> second_params(params.begin() + 20, params.begin() + 30);
//    for (int i = 0; i < first_params.size(); ++i) {
//        cout << i << " " << first_params[i] << endl;
//        first_surface.push_back(stof(first_params[i]));
//        cout << i << " " << first_surface[i] << endl;
//    }
//    for (int i = 0; i < second_params.size(); ++i) {
//        cout << i << " " << second_params[i] << endl;
//        second_surface.push_back(stof(second_params[i]));
//        cout << i << " " << second_surface[i] << endl;
//    }

//    EquationSystem eq({
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

//    AbstractSurface as(VF{1, 1, 0, 0, 0, 0, 2, -4, 2, 1});
// quite OK

//    AbstractSurface as(VF{1, 0, 0, 0, 0, 0, 0, 6, -8, 10});
// quite OK

    AbstractSurface as(VF{3, -7, 3, 8, -8, -8, 10, -14, -6, -8});

    as.getCanonical();




//    GLFWwindow *window = initWindow(800, 600);
//    if (nullptr != window) {
//        display(window);
//    }
//    glfwDestroyWindow(window);
//    glfwTerminate();

    return 0;
}