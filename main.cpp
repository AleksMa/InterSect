#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <vector>
#include "bits/stdc++.h"
#include "Primary/Point.h"
#include "Surfaces/Ellipsoid.h"
#include "Surfaces/ParaboloidElliptic.h"
#include "Evaluations/Equation.h"
#include "Evaluations/Calculations.cpp"
#include "SurfaceEquations/SurfaceEquation.h"
#include "Surfaces/HyperboloidOneSheet.h"
#include "Surfaces/HyperboloidTwoSheet.h"
#include "Evaluations/Matrix.h"
#include "Surfaces/ParaboloidHyperbolic.h"
#include "Surfaces/EmptySurface.h"
#include "InterSect/InterSect.h"


#define RESX 800
#define RESY 700
#define ORT_LENGTH 300
#define ORT_WIDTH 3

typedef tuple<float, float, float, float> tuple4f;
typedef vector<Point> VP;
typedef vector<float> VF;

using namespace std;



GLfloat a = -90.f, b = 0.f, c = -90.f;
//GLfloat a = 0.f, b = 0.f, c = 0.f;
GLfloat x = 0.f, y = 0.f, z = 0.f;

bool equal_surfaces = false;
bool transp = false;
bool hide = false;
bool typefl = true;
bool orts = true;

vector<float> first_coefs, second_coefs;

shared_ptr<SurfaceEquation> first_equation, second_equation;

shared_ptr<AbstractSurface> first_surface, second_surface;



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

        if (key == GLFW_KEY_0) {
            a = -90.f;
            b = 0.f;
            c = -90.f;
        }
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
    }
}

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
            glBegin(GL_QUADS);
            for (auto p : Vertices) {
                glVertex3f(p.x + x, p.y + y, p.z + z);
            }
            glEnd();
        }
    }
}

void draw_intersect(VP intersect) {
    glColor4f(0, 0, 1, 1);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (Point p : intersect) {
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

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

void display(GLFWwindow *window) {

    vector<Point> vertices_first, vertices_second, intersection;
    InterSect interSect(first_equation, second_equation, first_surface, second_surface);

    vertices_second = second_surface->make_mash();

    if(!equal_surfaces){
        vertices_first = first_surface->make_mash();
        intersection = interSect.create_intersect();
    }

    while (!glfwWindowShouldClose(window)) {
        GLint windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        glClearColor(1, 1, 1, 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2,
                windowHeight / 2, -(windowWidth + windowHeight) / 4, (windowWidth + windowHeight) / 4);

        glMatrixMode(GL_MODELVIEW_MATRIX);


        glRotatef(a, 1, 0, 0);
        glRotatef(b, 0, 1, 0);
        glRotatef(c, 0, 0, 1);


        if (orts) {
            render_x_ort();
            render_y_ort();
            render_z_ort();
        }

        if (!equal_surfaces) {
            glPushMatrix();

            glEnd();

            glPushMatrix();
            float mulm[] = {1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1};

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    mulm[4 * i + j] = first_equation->get_mul_matrix()[j][i];   // transp
                }
                mulm[12 + i] = first_equation->get_additional_vector()[i];
            }

            glMultMatrixf(mulm);

            draw_intersect(intersection);

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

        tuple4f second_color = (equal_surfaces ? tuple4f{0, 0, 1, 0.3} : tuple4f{1, 0, 0, 0.3});

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
        first_coefs.push_back(stof(first_params[i]));
    }
    for (int i = 0; i < second_params.size(); ++i) {
        second_coefs.push_back(stof(second_params[i]));
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
    return new EmptySurface();
}

bool check_equal(){
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

    return equals_temp;
}

int main(int argc, char **argv) {

    read_equations();

    equal_surfaces = check_equal();

    first_equation = std::make_shared<SurfaceEquation>(first_coefs);
    second_equation = std::make_shared<SurfaceEquation>(second_coefs);

    first_equation->canonizate();
    cout << "FIRST TYPE: " << first_equation->get_type() << endl;
    first_surface = shared_ptr<AbstractSurface>(make_surface(*first_equation));

    second_equation->canonizate();
    cout << "SECOND TYPE: " << second_equation->get_type() << endl;
    second_surface = shared_ptr<AbstractSurface>(make_surface(*second_equation));


    GLFWwindow *window = initWindow(RESX, RESY);
    if (nullptr != window) {
        display(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}