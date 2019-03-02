#include <ctime>
#include <stdlib.h>
#include <iostream>
void addMultOfBlocks(double *A, double *B, double* C, int N, int lda) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int q = 0; q < N; ++q) {
                C[i*lda + j] += A[i*lda + q] * B[q*lda + j];
            }
        }
    }
}
void clearMatrix(double *C, int N, int lda) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i*lda + j] = 0.0;
        }
    }
}
void cannon(double *A, double *B, double* C, int n, int q) {
    int blocksize = n / q;
    for (int i = 0; i < q; ++i) {
        for (int j = 0; j < q; ++j) {
            clearMatrix(&C[(i*n + j)*blocksize], blocksize, n);
            for (int k = 0; k < q; ++k) {
                addMultOfBlocks(&A[(i*n + (j+i+k)%q)*blocksize],
                    &B[(((i+j+k)%q)*n + j)*blocksize],
                    &C[(i*n + j)*blocksize], blocksize, n);
            }
        }
    }
}
int main(int argc, char** argv) {
    int n = 4, q = 2;
    srand(static_cast<unsigned int>(time(NULL)));
    if (argc == 3) {
        n = atoi(argv[1]);
        q = atoi(argv[2]);
    }
    double *A, *B, *C1, *C2;
    A = new double[n*n];
    B = new double[n*n];
    C1 = new double[n*n];
    C2 = new double[n*n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i*n + j] = rand_r() % 10;
            B[i*n + j] = rand_r() % 10;
            C1[i*n + j] = 0.0;
        }
    }
    addMultOfBlocks(A, B, C1, n, n);
    cannon(A, B, C2, n, q);
    double s = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (s < abs(C1[i*n + j] - C2[i*n + j])) s = abs(C1[i*n + j] - C2[i*n + j]);
        }
    }
    std::cout << s;
    return 0;
}
