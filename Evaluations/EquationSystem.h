//
// Created by alexey on 31.01.2020.
//

#ifndef INTERSECT_EQUATIONSYSTEM_H
#define INTERSECT_EQUATIONSYSTEM_H

#include <vector>

using namespace std;

typedef vector<float> VF;


class EquationSystem {
    const int M = 3, N = 4;
    vector<vector<float>> equation_system;

public:
    EquationSystem(vector<VF> system);
    VF solve();
};


#endif //INTERSECT_EQUATIONSYSTEM_H
