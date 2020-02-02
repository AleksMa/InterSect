//
// Created by alexey on 30.01.2020.
//

#ifndef INTERSECT_QUADRICEQUATION_H
#define INTERSECT_QUADRICEQUATION_H

#include <vector>

using namespace std;

typedef vector<float> VF;


struct QuadricEquation {
    const int size = 10;
    vector<float> equation;
    explicit QuadricEquation(VF coef);
    QuadricEquation();

    QuadricEquation(const QuadricEquation &other) {
        this->equation = other.equation;
    }

    QuadricEquation &operator= (const QuadricEquation &other) {
        if (this == &other) {
            this->equation = other.equation;
        }
        return *this;
    }

    float &XX();
    float &YY();
    float &ZZ();
    float &XY();
    float &XZ();
    float &YZ();
    float &X();
    float &Y();
    float &Z();
    float &D();
    void print();
};


#endif //INTERSECT_QUADRICEQUATION_H
