#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

static inline double f(double x) {
    return 4.0 / (1.0 + x * x);
}

double pi_trapezios(long long n) {
    double h = 1.0 / (double)n;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum) schedule(static)
    for (long long i = 1; i < n; i++) {
        sum += f(i * h);
    }

    sum += (f(0.0) + f(1.0)) / 2.0;
    return sum * h;
}

double pi_monte_carlo(long long n) {
    long long dentro = 0;

    #pragma omp parallel reduction(+:dentro)
    {
        unsigned int seed = (unsigned int)(omp_get_thread_num() * 1234567 + 42);
        long long local = 0;

        #pragma omp for schedule(static)
        for (long long i = 0; i < n; i++) {
            seed = seed * 1664525 + 1013904223;
            double x = (seed & 0x7fffffff) / (double)0x7fffffff;
            seed = seed * 1664525 + 1013904223;
            double y = (seed & 0x7fffffff) / (double)0x7fffffff;
            if (x * x + y * y <= 1.0)
                local++;
        }
        dentro += local;
    }

    return 4.0 * (double)dentro / (double)n;
}

int main(void) {
    long long N_integ = 10000000LL;
    long long N_mc    = 100000000LL;

    double t0, t1, pi_val;

    printf("Threads: %d\n", omp_get_max_threads());
    printf("Pi real: %.15f\n\n", M_PI);

    t0 = omp_get_wtime();
    pi_val = pi_trapezios(N_integ);
    t1 = omp_get_wtime();
    printf("Trapezios  n=%-10lld  pi=%.15f  erro=%.2e  tempo=%.4fs\n",
           N_integ, pi_val, fabs(pi_val - M_PI), t1 - t0);

    t0 = omp_get_wtime();
    pi_val = pi_monte_carlo(N_mc);
    t1 = omp_get_wtime();
    printf("MonteCarlo n=%-10lld  pi=%.15f  erro=%.2e  tempo=%.4fs\n",
           N_mc, pi_val, fabs(pi_val - M_PI), t1 - t0);

    return 0;
}