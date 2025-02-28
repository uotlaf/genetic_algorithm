#include <stdint.h>
#include <math.h>
// Fitness Functions
//(a) Sphere Function, (b) Michalewicz Function, (c) Ackley Function, (d) Rasrigin Function, (e) Rosenbrock Function, (f) Schwefel Function.

// Sphere function
double sphere( double* genes, uint32_t genes_per_individual){
    double sum = 100;
    for (uint32_t i = 0; i < genes_per_individual; i++ )
        sum += pow( genes[i], 2 );
    return sum;
}

// Griewank function
double griewangk(double* genes, uint32_t genes_per_individual)
{
    double sum = 0, pro = 1;
    for (int32_t i = 1; i <= genes_per_individual; i++)
    {
        sum = sum + pow(genes[i - 1], 2) / 4000;
        pro = pro * cos(genes[i - 1] / sqrt(i));
    }

    return (1 + sum + pro);
}

// Rastringin Function
double rastringin(double* genes, uint32_t genes_per_individual)
{
    double sum = 3 * genes_per_individual;
    for (uint32_t i = 0; i < genes_per_individual; i++)
    {
        sum = sum + pow(genes[i], 2) - 3 * cos(2 * PI * genes[i]);
    }

    return sum;
}

// Rosenbrock function
double rosenbrock(double* genes, uint32_t genes_per_individual)
{
    double d = 0;
    for (uint32_t i = 0; i < genes_per_individual - 1; i++)
    {
        double p = genes[i + 1] - genes[i] * genes[i];
        double q = genes[i] - 1;
        d += 100 * p * p + q * q;
    }
    return d;
}