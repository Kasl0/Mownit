#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_blas.h>

//gcc -O0 -Wall -I/usr/local/include Zad2.c main -lgsl

int n;

// Mnożenie macierzy - wersja naiwna
void naive_multiplication(double A[][n], double B[][n], double C[][n]) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            C[i][j] = 0;

            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Ulepszona wersja
void better_multiplication(double A[][n], double B[][n], double C[][n]) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// BLAS
void blas(double a[n*n], double b[n*n], double c[n*n]) {

    gsl_matrix_view A = gsl_matrix_view_array(a, n, n);
    gsl_matrix_view B = gsl_matrix_view_array(b, n, n);
    gsl_matrix_view C = gsl_matrix_view_array(c, n, n);

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &A.matrix, &B.matrix, 0.0, &C.matrix);
}

int main() {

    int num_measurements = 10;

    FILE *fp = fopen("results.csv", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(fp, "matrix_size,function_name,time\n");

    for (int matrix_size = 1; matrix_size <= 300; matrix_size+=5) {

        n = matrix_size;

        for (int i = 0; i < num_measurements; i++) {

            double a[matrix_size][matrix_size];
            double b[matrix_size][matrix_size];
            double c[matrix_size][matrix_size];
            double a_blas[matrix_size * matrix_size];
            double b_blas[matrix_size * matrix_size];
            double c_blas[matrix_size * matrix_size];

            srand(time(NULL));
            
            for (int i = 0; i < matrix_size; i++) {
                for (int j = 0; j < matrix_size; j++) {
                    a[i][j] = (double) rand() / RAND_MAX;
                    a_blas[i*matrix_size + j] = a[i][j];
                }
            }
            for (int i = 0; i < matrix_size; i++) {
                for (int j = 0; j < matrix_size; j++) {
                    b[i][j] = (double) rand() / RAND_MAX;
                    b_blas[i*matrix_size + j] = b[i][j];
                }
            }
            
            struct timespec naive_start, naive_end;
            clock_gettime(CLOCK_REALTIME, &naive_start);
                naive_multiplication(a, b, c);
            clock_gettime(CLOCK_REALTIME, &naive_end);
            double naive_time = (naive_end.tv_sec - naive_start.tv_sec) + (naive_end.tv_nsec - naive_start.tv_nsec) * 0.000000001;
            fprintf(fp, "%d,%s,%f\n", matrix_size, "naive_multiplication", naive_time);

            struct timespec better_start, better_end;
            clock_gettime(CLOCK_REALTIME, &better_start);
                better_multiplication(a, b, c);
            clock_gettime(CLOCK_REALTIME, &better_end);
            double better_time = (better_end.tv_sec - better_start.tv_sec) + (better_end.tv_nsec - better_start.tv_nsec) * 0.000000001;
            fprintf(fp, "%d,%s,%f\n", matrix_size, "better_multiplication", better_time);

            struct timespec blas_start, blas_end;
            clock_gettime(CLOCK_REALTIME, &blas_start);
                blas(a_blas, b_blas, c_blas);
            clock_gettime(CLOCK_REALTIME, &blas_end);
            double blas_time = (blas_end.tv_sec - blas_start.tv_sec) + (blas_end.tv_nsec - blas_start.tv_nsec) * 0.000000001;
            fprintf(fp, "%d,%s,%f\n", matrix_size, "BLAS", blas_time);
            
        }
    }

    fclose(fp);
    return 0;
}
