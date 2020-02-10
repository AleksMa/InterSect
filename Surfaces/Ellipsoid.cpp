//
// Created by alexey on 18.12.2019.
//

#include <cmath>
#include <iostream>
#include "Ellipsoid.h"

Ellipsoid::Ellipsoid(float a, float b, float c) : A(a), B(b), C(c) {
    cout << "new Ellipsoid: " << endl;
    cout << A << endl;
    cout << B << endl;
    cout << C << endl;
}

vector<Point> Ellipsoid::make_vertices() {
    N = C / 20 + 5;
    M = 20;
    vector<Point> vertices;
    const float pi = 3.14159265358979323846;
    vertices.emplace_back(0.f, 0.f, C);
    for (int i = 1; i < 2 * N; i++) {
        for (int j = 0; j < M; j++) {
            vertices.emplace_back(
                    A * sin((float) i * M_PI / N / 2) * cos((float) j * M_PI * 2 / M),
                    B * sin((float) i * M_PI / N / 2) * sin((float) j * M_PI * 2 / M),
                    C * cos((float) i * M_PI / N / 2)
            );
        }
    }
    vertices.emplace_back(0.f, 0.f, -C);
    return vertices;
}

vector<Point> Ellipsoid::make_mash() {
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


    for (int i = 0; i < 2 * N - 2; i++) {
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

    for (int i = 0; i < M; i++) {
        Point p = Vertices[Vertices.size() - 1];
        Point p1 = Vertices[M * (2 * N - 2) + i + 1];
        Point p2 = Vertices[(i == M - 1 ? M * (2 * N - 2) + 1 : M * (2 * N - 2) + i + 2)];
        finalVerticies.push_back(p);
        finalVerticies.push_back(p);
        finalVerticies.push_back(p1);
        finalVerticies.push_back(p2);
    }

//    for (int j = 0; j < finalVerticies.size_x(); ++j) {
//        cout << finalVerticies[j].x << " " << finalVerticies[j].y << " " << finalVerticies[j].z << std::endl;
//    }


    if (this->B == 0) {
        return vector<Point>{finalVerticies[0]};
    }
    return finalVerticies;
}

float Ellipsoid::max_z() {
    return C;
}

float Ellipsoid::min_z() {
    return -C;
}



