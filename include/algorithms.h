#pragma once
#include <stdint.h>
#include <genectics.h>

#define RANDOM rand() % 101 / 100.0

void populate(
    Population *Population,
    double (*fitness_function)(double *genes, uint32_t genes_per_individual));

void genetic_algorithm(
    Population* population, 
    double (*fitness_function)(double *genes, uint32_t genes_per_individual));