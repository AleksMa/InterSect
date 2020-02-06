//
// Created by alexey on 06.02.2020.
//

#ifndef INTERSECT_MATRIX_H
#define INTERSECT_MATRIX_H

#include <vector>

using namespace std;


class Matrix {
private:
    vector <vector<float>> A;
    int dim;

public:
    Matrix(vector <vector<float>> a);

    vector <vector<float>> reverse();
};


#endif //INTERSECT_MATRIX_H
