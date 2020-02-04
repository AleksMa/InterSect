#include <cmath>
#include <iostream>
#include "Paraboloid.h"

Paraboloid::Paraboloid(float a, float b) : A(a), B(b) {}

vector<Point> Paraboloid::make_vertices() {
    N = 130 / 10 + 5;
    M = 20;
    vector<Point> vertices;
    const float pi = 3.14159265358979323846;
    vertices.push_back(Point(0.f, 0.f, 0.f));
    for (int i = 2; i < N; i++) {
        float u = i;
        for (int j = 0; j < M; j++) {
            vertices.emplace_back(
                    A * u * cos((float) j * pi * 2 / M),
                    B * u * sin((float) j * pi * 2 / M),
                    u * u
            );
        }
    }
    return vertices;
}

vector<Point> Paraboloid::make_mash() {
    vector<Point> Vertices = this->make_vertices();
    vector<Point> finalVerticies;
    for (int i = 0; i < M; i++) {
        Point p = Vertices[0];
        Point p1 = Vertices[i + 1], p2 = Vertices[(i == M - 1 ? 1 : i + 2)];
        finalVerticies.push_back(p);
        finalVerticies.push_back(p);
        finalVerticies.push_back(p1);
        finalVerticies.push_back(p2);
    }


    for (int i = 0; i < N - 3; i++) {
        for (int j = 0; j < M; j++) {
            Point p1 = Vertices[M * i + j + 1];
            Point p2 = Vertices[(j == M - 1 ? M * i + 1 : M * i + j + 2)];
            Point p3 = Vertices[M * (i + 1) + j + 1];
            Point p4 = Vertices[(j == M - 1 ? M * (i + 1) + 1 : M * (i + 1) + j + 2)];
            finalVerticies.push_back(p1);
            finalVerticies.push_back(p3);
            finalVerticies.push_back(p4);
            finalVerticies.push_back(p2);
        }
    }

//    for (int j = 0; j < finalVerticies.size(); ++j) {
//        cout << finalVerticies[j].x << " " << finalVerticies[j].y << " " << finalVerticies[j].z << std::endl;
//    }


    if (this->B == 0) {
        return vector<Point>{finalVerticies[0]};
    }
    return finalVerticies;
}



