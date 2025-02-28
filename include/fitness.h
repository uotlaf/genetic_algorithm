#pragma once
#include <stdint.h>

double sphere( double* genes, uint32_t genes_per_individual);
double griewangk(double* genes, uint32_t genes_per_individual);
double rastringin(double* genes, uint32_t genes_per_individual);
double rosenbrock(double* genes, uint32_t genes_per_individual);

static double (*fitness_functions[])(double[], uint32_t genes_per_individual) =
    {sphere, griewangk, rastringin, rosenbrock};