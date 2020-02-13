#include "ParaboloidHyperbolic.h"

#include <cmath>
#include <iostream>

ParaboloidHyperbolic::ParaboloidHyperbolic(float a, float b) : A(a), B(b) {
    size_x = 150;
    size_z = 50;
    M = 20;
    N = 1;
    K = 0;
}

vector<Point> ParaboloidHyperbolic::make_vertices() {
    vector<Point> vertices;
    float left = float(size_x * size_x) / (A * A);
    max = left;
    vertices.emplace_back(-size_x,
                          0,
                          left
    );
    int l;
    for (l = left; l > -size_z; l -= N) {
        N += 3;
        K++;
        vertices.emplace_back(-size_x,
                              sqrt(left - l) * B,
                              l
        );
    }
    size_z = -l - N;
    for (int i = -size_x + M; i <= size_x; i += M) {    // x
        N = 1;
        // xx / aa - yy / bb = zz
        left = float(i * i) / (A * A);
        vertices.emplace_back(i,
                              0,
                              left
        );
        int k = 0;
        for (int j = left; k < K; j -= N) { // z
            N += 3;
            k++;
            vertices.emplace_back(i,
                                  sqrt(left - (float) j) * B,
                                  j
            );
        }
    }
    return vertices;
}

vector<Point> ParaboloidHyperbolic::make_mash() {
    vector<Point> vertices = make_vertices();
    cout << "SIZE: " << vertices.size() << endl;
    vector<Point> final_vertices;

    for (int i = 0; i < 2 * size_x / M; ++i) {
        for (int j = 0; j < K; ++j) {
            Point p1 = vertices[(K + 1) * i + j];
            Point p2 = vertices[(K + 1) * i + j + 1];
            Point p3 = vertices[(K + 1) * (i + 1) + j];
            Point p4 = vertices[(K + 1) * (i + 1) + j + 1];
            final_vertices.push_back(p1);
            final_vertices.push_back(p2);
            final_vertices.push_back(p4);
            final_vertices.push_back(p3);
            p1.y *= -1;
            p2.y *= -1;
            p3.y *= -1;
            p4.y *= -1;
            final_vertices.push_back(p1);
            final_vertices.push_back(p2);
            final_vertices.push_back(p4);
            final_vertices.push_back(p3);
        }
    }

    return final_vertices;
}

float ParaboloidHyperbolic::max_z() {
    return size_x;
}

float ParaboloidHyperbolic::min_z() {
    return size_z;
}
