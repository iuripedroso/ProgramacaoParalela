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

    #pragma omp parallel for reduction(+:pi_aprox)
    for (int i = 0; i < N; i++)
    {
        double x = a + i * h;
        pi_aprox += funcao(x) * h;
    }

    printf("Valor aproximado de pi: %f\n", pi_aprox);

    return 0;
}