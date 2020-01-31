//
// Created by alexey on 31.01.2020.
//

#include <iostream>
#include "EquationSystem.h"

EquationSystem::EquationSystem(vector<VF> system): equation_system(system) {
    for (VF equation : equation_system) {
        while (equation.size() < N) {
            equation.push_back(0);
        }
    }
    while (equation_system.size() < M)
        equation_system.emplace_back(N, 0);
}

VF EquationSystem::solve() {

    for (int i = 0; i < M - 1; ++i) {
        for (int l = N - 1; l >= i; l--) {
            equation_system[i][l] /= equation_system[i][i];
        }
        for (int j = i + 1; j < M; j++) {
            float C = equation_system[j][i] / equation_system[i][i];
            for (int k = i; k < N; ++k) {
                equation_system[j][k] -= equation_system[i][k] * C;
            }
        }
    }

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << equation_system[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for (int i = M - 1; i > 0; --i) {
        for (int l = N - 1; l >= i; l--) {
            equation_system[i][l] /= equation_system[i][i];
        }
        for (int j = i - 1; j >= 0; j--) {
            float C = equation_system[j][i] / equation_system[i][i];
            for (int k = i; k < N; ++k) {
                equation_system[j][k] -= equation_system[i][k] * C;
            }
        }
    }

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << equation_system[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    VF ret;
    for (int m = 0; m < M; ++m) {
        ret.push_back(equation_system[m][N - 1]);
    }

    return ret;
}
