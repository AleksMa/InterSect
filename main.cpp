#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "bits/stdc++.h"
#include "Primary/Point.h"
#include "Surfaces/Ellipsoid.h"

using namespace std;

GLfloat a = 20.f, b = 0.f, c = 0.f;
GLfloat x = 0.f, y = 0.f, z = 0.f;
GLboolean fl = false;


//GLint radius = 100;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
            fl = !fl;
    }

    if (action = GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) {
            a -= 3.f;
        }
        if (key == GLFW_KEY_DOWN) {
            a += 3.f;
        }
        if (key == GLFW_KEY_RIGHT) {
            b -= 3.f;
        }

        if (key == GLFW_KEY_LEFT) {
            b += 3.f;
        }

        if (key == GLFW_KEY_KP_1) {
            c -= 3.f;
        }

        if (key == GLFW_KEY_RIGHT_SHIFT) {
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

vector<Point> makeSphere(GLint r, float A, float B, float C) {
    Ellipsoid ellipsoid(A, B, C, r * r);
    return ellipsoid.makeEllipsoidMash();
}

vector<Point> makeParaboloid(GLint r, float A, float B, float C) {
    Ellipsoid ellipsoid(A, B, C, r * r);
    return ellipsoid.makeEllipsoidMash();
}

void drawMySphere(vector<Point> Vertices) {

    static float alpha = 0;

    glTranslatef(x, y, z);

    glRotatef(a, 1, 0, 0);
    glRotatef(b, 0, 1, 0);
    glRotatef(c, 0, 0, 1);

    glTranslatef(-x, -y, -z);

    GLfloat r = 1.f;
    GLfloat g = 0.f;
    GLfloat b = 0.f;

    int type = (fl ? GL_POLYGON : GL_LINE_STRIP);

    //cout << Vertices.size() << endl;

    if (Vertices.size() < 4)
        type = GL_POINTS;
    glColor4f(0.f, 1.f, 0.f, 0.5);
    glPointSize(4);
    glLineWidth(1);

    //glEnable(GL_BLEND); //Enable blending.
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

    glBegin(type);
    for (auto p : Vertices) {
        glVertex3f(p.x + x, p.y + y, p.z + z);
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
    return window;
}


void display(GLFWwindow *window) {

    vector<Point> vertices = makeSphere(100, 200, 100, 150);

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

        drawMySphere(vertices);

        glfwSwapBuffers(window);
        glfwPollEvents();

        glPushMatrix();
    }
}


int main(int argc, char **argv) {
    GLFWwindow *window = initWindow(800, 600);
    if (nullptr != window) {
        display(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}