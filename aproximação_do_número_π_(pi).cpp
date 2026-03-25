#include <stdio.h>
#include <omp.h>
#include <iostream>
using namespace std;
#define N 100000

double funcao(double x)
{
    return 4.0 / (1.0 + x * x);
}

int main()
{
    double a = 0.0, b = 1.0;
    double h = (b - a) / N;
    double pi_aprox = 0.0;

    int total_threads = omp_get_max_threads();
    double acumulador[total_threads];

    for (int k = 0; k < total_threads; k++)
    {
        acumulador[k] = 0.0;
    }

#pragma omp parallel
    {
        int tid = omp_get_thread_num();

        for (int j = tid; j < N; j += total_threads)
        {
            double x = a + j * h;
            acumulador[tid] += funcao(x) * h;
        }

        printf("Thread %d parcial: %f\n", tid, acumulador[tid]);
    }

    for (int k = 0; k < total_threads; k++)
    {
        pi_aprox += acumulador[k];
    }

    printf("Valor aproximado de pi: %f\n", pi_aprox);

    return 0;
}