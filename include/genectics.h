#pragma once
#include <stdint.h>
#include <stdbool.h>

// Variáveis de seleção e cruzamento
#define XALPHA 0.25

// Mutação
#define PMUTAC 50
#define MUTATION_CHANGE_TYPE_CHANCE 0.05

// Pesquisa local
#define DEFAULT_ITER_LOCAL_SEARCH 10
#define MIN_ITERATIONS 1000

// Variáveis relacionadas
#define NEIGHBORS 2

// Structs usadas no trabalho todo
typedef struct _chromossome_ {
    double*           genes;
    double            fitness;
    double            old_fitness;
    uint8_t           selections;
    uint8_t           old_selections;
    bool              mutated;
} Chromossome;

typedef struct _population_ {
    Chromossome     individuals[MAX_POPULATION];
    double          sum_fitness;
    uint64_t        size;
    uint32_t        genes_per_individual;

    uint32_t        current_generation;
    uint32_t        max_generation;
//    uint32_t        best;
//    uint32_t        worst;
    uint32_t        mutation_chance;
    uint32_t        crossover_chance;
//    uint32_t        equals;
//    uint32_t        best_generation;

//    double          lower_bounds;
//    double          upper_bounds;

    double          lower_gene_limit; 
    double          upper_gene_limit;
} Population;