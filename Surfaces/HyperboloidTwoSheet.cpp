#include <cmath>
#include <iostream>
#include "HyperboloidTwoSheet.h"

HyperboloidTwoSheet::HyperboloidTwoSheet(float a, float b, float c) : A(a), B(b), C(c) {
    size = 250;
    M = 20;
    N = 1;
    K = 0;
}

vector<Point> HyperboloidTwoSheet::make_vertices() {
    vector<Point> vertices;
    vertices.emplace_back(0.f, 0.f, C);
    for (int i = C + N; i <= size; i += N) {
        N += 3;
        K++;
        // xx / aa + yy / bb = -1 + zz/cc
        float right = -1.f + (float) (i * i) / (C * C);
        for (int j = 0; j < M; ++j) {
            vertices.emplace_back(A * sqrt(right) * cos((float) j * M_PI * 2 / M),
                                  B * sqrt(right) * sin((float) j * M_PI * 2 / M),
                                  i
            );
        }
    }
    N = 1;
    vertices.emplace_back(0.f, 0.f, -C);
    for (int i = C + N; i <= size; i += N) {
        N += 3;
        // xx / aa + yy / bb = -1 + zz/cc
        float right = -1.f + (float) (i * i) / (C * C);
        for (int j = 0; j < M; ++j) {
            vertices.emplace_back(A * sqrt(right) * cos((float) j * M_PI * 2 / M),
                                  B * sqrt(right) * sin((float) j * M_PI * 2 / M),
                                  -i
            );
        }
    }
    return vertices;
}

vector<Point> HyperboloidTwoSheet::make_mash() {
    vector<Point> vertices = make_vertices();
    cout << "SIZE: " << vertices.size() << endl;
    vector<Point> final_vertices;

    for (int i = 0; i < M; i++) {
        Point p = vertices[0];
        Point p1 = vertices[i + 1], p2 = vertices[(i == M - 1 ? 1 : i + 2)];
        final_vertices.push_back(p);
        final_vertices.push_back(p);
        final_vertices.push_back(p1);
        final_vertices.push_back(p2);
    }

    for (int i = 0; i < K - 1; ++i) {
        for (int j = 0; j < M; j++) {
            Point p1 = vertices[M * i + j + 1];
            Point p2 = vertices[(j == M - 1 ? M * i + 1 : M * i + j + 2)];
            Point p3 = vertices[M * (i + 1) + j + 1];
            Point p4 = vertices[(j == M - 1 ? M * (i + 1) + 1 : M * (i + 1) + j + 2)];
            final_vertices.push_back(p1);
            final_vertices.push_back(p3);
            final_vertices.push_back(p4);
            final_vertices.push_back(p2);
        }
    }

    // less zero

    const int less_zero_inc = K * M + 1;

    for (int i = 0; i < M; i++) {
        Point p = vertices[less_zero_inc];
        Point p1 = vertices[less_zero_inc + i + 1],
                p2 = vertices[(i == M - 1 ? less_zero_inc + 1 : less_zero_inc + i + 2)];
        final_vertices.push_back(p);
        final_vertices.push_back(p);
        final_vertices.push_back(p1);
        final_vertices.push_back(p2);
    }

    for (int i = 0; i < K - 1; ++i) {
        for (int j = 0; j < M; j++) {
            Point p1 = vertices[less_zero_inc + M * i + j + 1];
            Point p2 = vertices[(j == M - 1 ? less_zero_inc + M * i + 1 : less_zero_inc + M * i + j +
                                                                                          2)];
            Point p3 = vertices[less_zero_inc + M * (i + 1) + j + 1];
            Point p4 = vertices[(j == M - 1 ? less_zero_inc + M * (i + 1) + 1 : less_zero_inc +
                                                                                                M * (i + 1) + j + 2)];
            final_vertices.push_back(p1);
            final_vertices.push_back(p3);
            final_vertices.push_back(p4);
            final_vertices.push_back(p2);
        }
    }

    return final_vertices;
}

float HyperboloidTwoSheet::max_z() {
    return size;
}

float HyperboloidTwoSheet::min_z() {
    return -size;
}
