//
// Created by alexey on 05.02.2020.
//

#include <math.h>
#include <iostream>
#include "HyperboloidOneSheet.h"

HyperboloidOneSheet::HyperboloidOneSheet(float a, float b, float c) : A(a), B(b), C(c) {
    size = 200;
    M = 20;
    N = 20;
}

vector<Point> HyperboloidOneSheet::make_vertices() {
    vector<Point> vertices;
    for (int i = size; i >= -size; i -= N) {
        // xx / aa + yy / bb = 1 + zz/cc
        float right = 1.f + (float) (i * i) / (C * C);
        for (int j = 0; j < M; ++j) {
            vertices.emplace_back(A * sqrt(right) * cos((float) j * M_PI * 2 / M),
                                  B * sqrt(right) * sin((float) j * M_PI * 2 / M),
                                  i
            );
        }
    }
    return vertices;
}

vector<Point> HyperboloidOneSheet::make_mash() {
    vector<Point> vertices = make_vertices();
    cout << "SIZE: " << vertices.size() << endl;
    vector<Point> final_vertices;

    for (int i = 0; i < 2 * size / N - 1; ++i) {
        for (int j = 0; j < M; j++) {
            Point p1 = vertices[M * i + j];
            Point p2 = vertices[(j == M - 1 ? M * i : M * i + j + 1)];
            Point p3 = vertices[M * (i + 1) + j];
            Point p4 = vertices[(j == M - 1 ? M * (i + 1) : M * (i + 1) + j + 1)];
            final_vertices.push_back(p1);
            final_vertices.push_back(p3);
            final_vertices.push_back(p4);
            final_vertices.push_back(p2);
        }
    }
    return final_vertices;
}

float HyperboloidOneSheet::max_z() {
    return size;
}

float HyperboloidOneSheet::min_z() {
    return -size + N;
}
