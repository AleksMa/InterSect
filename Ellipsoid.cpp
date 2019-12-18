//
// Created by alexey on 18.12.2019.
//

#include <cmath>
#include "Ellipsoid.h"

Ellipsoid::Ellipsoid(float a, float b, float c, float d) : A(a), B(b), C(c), D(d) {}

Ellipsoid::Ellipsoid(float a, float b, float c, float cX, float cY, float cZ, int R, int n, int m) : A(a), B(b), C(c), cX(cX),
                                                                                              cY(cY), cZ(cZ), R(R), N(n),
                                                                                              M(m) {}
vector<class Point> Ellipsoid::makeEllipsoidVerticies() {
    R = int(sqrt(D));
    N = R / 20;
    M = 20;
    vector<Point> Vertices;
    const float pi = 3.14159265358979323846;
    Vertices.push_back(Point(0.f, B * R, 0.f));
    for (int i = N - 1; i >= 0; i--) {
        float y = R * i / N;
        float radius = sqrt(static_cast<float>(R*R) - (y*y));
        for (int j = 0; j < M; j++) {
            Vertices.push_back(Point(A * radius * cos(j * pi * 2 / M),  B * y, C * radius * sin(j * pi * 2 / M)));
            //Vertices.push_back(Point(radius * cos(j * pi * 2 / M), - radius / M * i, radius * sin(j * pi * 2 / M)));
        }
    }

    for (int i = 0; i < N; i++) {
        float y = R * i / N;
        float radius = sqrt(static_cast<float>(R*R) - (y*y));
        for (int j = 0; j < M; j++) {
            Vertices.push_back(Point(A * radius * cos(j * pi * 2 / M), B * -y, C * radius * sin(j * pi * 2 / M)));
        }
    }

    Vertices.push_back(Point(0.f, B * -R, 0.f));
    return Vertices;
}

vector<class Point> Ellipsoid::makeEllipsoidMash() {
    vector<Point> Vertices = this->makeEllipsoidVerticies();
    vector<Point> finalVerticies;
    for (int i = 0; i < M; i++) {

        Point p = Vertices[0];
        Point p1 = Vertices[i + 1], p2 = Vertices[(i == M - 1 ? 1 : i + 2)];
        finalVerticies.push_back(p);
        finalVerticies.push_back(p1);
        finalVerticies.push_back(p2);
    }


    for (int i = 0; i < 2 * N - 1; i++) {
        for (int j = 0; j < M; j++) {
            Point p1 = Vertices[M * i + j + 1], p2 = Vertices[(j == M - 1 ? M * i + 1 : M * i + j + 2)];
            Point p3 = Vertices[M * (i + 1) + j + 1], p4 = Vertices[(j == M - 1 ? M * (i + 1) + 1 : M * (i + 1) + j +
                                                                                                    2)];
            finalVerticies.push_back(p1);
            finalVerticies.push_back(p3);
            finalVerticies.push_back(p4);
            finalVerticies.push_back(p2);
        }
    }

    for (int i = 0; i < M; i++) {
        Point p = Vertices[Vertices.size() - 1];
        Point p1 = Vertices[M * (2 * N - 1) + i + 1], p2 = Vertices[(i == M - 1 ? M * (2 * N - 1) + 1 :
                                                                     M * (2 * N - 1) + i + 2)];
        finalVerticies.push_back(p);
        finalVerticies.push_back(p1);
        finalVerticies.push_back(p2);
    }
    return finalVerticies;
}



