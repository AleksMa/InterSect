//
// Created by alexey on 31.01.2020.
//

#include <iostream>
#include "EquationSystem.h"
#include "Calculations.cpp"

EquationSystem::EquationSystem(vector<VF> system) : equation_system(system) {
    for (VF equation : equation_system) {
        while (equation.size() < N) {
            equation.push_back(0);
        }
    }
    while (equation_system.size() < M)
        equation_system.emplace_back(N, 0);
}

VF EquationSystem::solve() {

    VF ret;

    //print();

    lower_course();
    //print();

    if (is_homogen()) {
        int zerolines = zeroline_count();
        int count = 0;
        for (int i = 0; i < M - zerolines; i++) {
            if (is_zeroline(i)) {
                //cout << i << endl;
                swap(equation_system[i], equation_system[M - zerolines + count]);
                count++;
                i--;
            }
        }
        //print();

        for (int i = M - 1 - zerolines; i > 0; --i) {
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
        //print();

        //cout << zerolines << endl << endl;
        int non_zero = M - zerolines;
        for (int i = 0; i < non_zero; ++i) {
            equation_system[i][non_zero] *= -1;
            for (int j = non_zero + 1; j < N; ++j) {
                equation_system[i][non_zero] -= equation_system[i][j];
            }
        }
        //print();
        for (int m = 0; m < non_zero; ++m) {
            ret.push_back(equation_system[m][non_zero]);
        }
        for (int n = non_zero; n < N - 1; ++n) {
            ret.push_back(1);
        }
    } else {
        upper_course();
        //print();
        for (int m = 0; m < M; ++m) {
            ret.push_back(equation_system[m][N - 1]);
        }
    }

//    cout << "SOLVED: " << endl;
//    for (auto solved_value : ret) {
//        cout << solved_value << " ";
//    }
//    cout << endl << endl;

    return ret;
}

void EquationSystem::lower_course() {
    int k = 0;
    for (int i = 0; i < M - 1; ++i) {
        if (is_zero(equation_system[i][i])  && i < M - 1) {
            int c = i + 1;
            while (i < M) {
                if (!is_zero(equation_system[c][i])) {
                    swap(equation_system[i], equation_system[c]);
                    break;
                }
                i++;
            }
            if (i >= M)
                return;
        }
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
}

void EquationSystem::upper_course() {
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
}

void EquationSystem::print() {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << equation_system[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool EquationSystem::is_homogen() {
    bool is = true;
    for (int i = 0; i < M; ++i) {
        is = is && is_zero(equation_system[i][N - 1]);
    }
    return is;
}

bool EquationSystem::is_zeroline(int i) {
    bool is = true;
    for (int j = 0; j < N; ++j) {
        is = is && is_zero(equation_system[i][j]);
    }
    return is;
}

int EquationSystem::zeroline_count() {
    int k = 0;
    for (int i = 0; i < M; ++i) {
        if (is_zeroline(i)) {
            k++;
        }
    }
    return k;
}
