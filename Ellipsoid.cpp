//
// Created by alexey on 18.12.2019.
//

#include <cmath>
#include "Ellipsoid.h"

Ellipsoid::Ellipsoid(float a, float b, float c) : A(a), B(b), C(c) {}

Ellipsoid::Ellipsoid(float a, float b, float c, float cX, float cY, float cZ, int R, int n, int m) : A(a), B(b), C(c), cX(cX),
                                                                                              cY(cY), cZ(cZ), R(R), N(n),
                                                                                              M(m) {}
vector<class Point> Ellipsoid::makeEllipsoid() {
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


