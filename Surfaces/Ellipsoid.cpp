//
// Created by alexey on 18.12.2019.
//

#include <cmath>
#include "Ellipsoid.h"

Ellipsoid::Ellipsoid(float a, float b, float c, float d) : A(a), B(b), C(c), D(d) {}

vector<Point> Ellipsoid::makeEllipsoidVertices() {
    N = C / 20 + 5;
    M = 20;
    vector<Point> vertices;
    const float pi = 3.14159265358979323846;
    vertices.push_back(Point(0.f, 0.f, C));
    for (int i = 1; i < 2 * N; i++) {
        for (int j = 0; j < M; j++) {
            vertices.push_back(Point(
                    A * sin(i * pi / N) * cos(j * pi * 2 / M),
                    B * sin(i * pi / N) * sin(j * pi * 2 / M),
                    C * cos(i * pi / N)
            ));
            //vertices.push_back(Point(radius * cos(j * pi * 2 / M), - radius / M * i, radius * sin(j * pi * 2 / M)));
        }
    }
    vertices.push_back(Point(0.f, 0.f, -C));
    return vertices;
}

vector<Point> Ellipsoid::makeEllipsoidMash() {
    vector<Point> Vertices = this->makeEllipsoidVertices();
    vector<Point> finalVerticies;
    for (int i = 0; i < M; i++) {

        Point p = Vertices[0];
        Point p1 = Vertices[i + 1], p2 = Vertices[(i == M - 1 ? 1 : i + 2)];
        finalVerticies.push_back(p);
        finalVerticies.push_back(p1);
        finalVerticies.push_back(p2);
    }


//    for (int i = 0; i < 2 * N - 1; i++) {
//        for (int j = 0; j < M; j++) {
//            Point p1 = Vertices[M * i + j + 1], p2 = Vertices[(j == M - 1 ? M * i + 1 : M * i + j + 2)];
//            Point p3 = Vertices[M * (i + 1) + j + 1], p4 = Vertices[(j == M - 1 ? M * (i + 1) + 1 : M * (i + 1) + j +
//                                                                                                    2)];
//            finalVerticies.push_back(p1);
//            finalVerticies.push_back(p3);
//            finalVerticies.push_back(p4);
//            finalVerticies.push_back(p2);
//        }
//    }

    for (int i = 0; i < M; i++) {
        Point p = Vertices[Vertices.size() - 1];
        Point p1 = Vertices[M * (2 * N - 2) + i + 1];
        Point p2 = Vertices[(i == M - 1 ? M * (2 * N - 2) + 1 : M * (2 * N - 2) + i + 2)];
        finalVerticies.push_back(p);
        finalVerticies.push_back(p1);
        finalVerticies.push_back(p2);
    }


    if (this->B == 0) {
        return vector<Point>{finalVerticies[0]};
    }
    return finalVerticies;
}



