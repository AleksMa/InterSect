#include "Matrix.h"

#include <utility>
#include <cmath>

static const float eps = 1e-7;

Matrix::Matrix(vector<vector<float>> a) : A(std::move(a)), dim(3) {}

vector<vector<float>> Matrix::reverse() {
    vector<vector<float>> A_ = {{1, 0, 0},
                                {0, 1, 0},
                                {0, 0, 1}};
    int i, j, k, leader_pos;
    float leader, temp;
    for (i = 0; i < dim; i++) {
        leader = A[i][i];
        leader_pos = i;
        for (j = i; j < dim; j++)
            if (fabs(A[j][i]) > fabs(leader)) {
                leader = A[j][i];
                leader_pos = j;
            }
        if (fabs(leader) > eps) {
            for (j = 0; j < dim; j++) {
                temp = A[i][j];
                A[i][j] = A[leader_pos][j];
                A[leader_pos][j] = temp;

                temp = A_[i][j];
                A_[i][j] = A_[leader_pos][j];
                A_[leader_pos][j] = temp;
            }
            for (j = 0; j < dim; j++) {
                if (i != j) {
                    temp = (A[j][i] / A[i][i]);
                    for (k = 0; k < dim; k++) {
                        A[j][k] -= A[i][k] * temp;
                        A_[j][k] -= A_[i][k] * temp;
                    }
                }
            }
        }
    }
    float t4;
    for (i = 0; i < dim; i++) {
        t4 = A[i][i];
        A[i][i] /= t4;
        for (k = 0; k < dim; k++)
            A_[i][k] /= t4;
    }
    return A_;
}
