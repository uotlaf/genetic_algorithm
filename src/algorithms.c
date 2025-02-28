#include <genectics.h>
#include <algorithms.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <mpi.h>
// Base algorithms


void populate(
    Population *population,
    double (*fitness_function)(double *genes, uint32_t genes_per_individual))
{
    assert(population);
    assert(population->size != 0);
    assert(population->genes_per_individual != 0);
    assert(population->upper_gene_limit != 0);
    assert(fitness_function);

    double fitness_sum = 0;

    for (uint32_t current_individual = 0; current_individual < population->size; current_individual++) {
        // Allocation phase
        population->individuals[current_individual].genes = malloc(sizeof(double) * population->genes_per_individual);
        assert(population->individuals[current_individual].genes);

        // Default values
        population->individuals[current_individual].sels = 0;

        // Populate genes
        for (uint32_t current_gene = 0; current_gene < population->genes_per_individual; current_gene++ ) {
            // Generate random value from lower limit
            double random_value = population->lower_gene_limit + (RANDOM * (population->upper_gene_limit - population->lower_gene_limit));
            population->individuals[current_individual].genes[current_gene] = random_value;
        }
        
        // Calculate fitness function
        double response_fitness = fitness_function(population->individuals[current_individual].genes, population->genes_per_individual);
        population->individuals[current_individual].fitness = response_fitness;

        fitness_sum += response_fitness;
    }

    population->sum_fitness = fitness_sum;
    population->current_generation = 0;
}


// Mutation - If chance > mutation rate, 
void mutation(
    Population* population,
    uint64_t individual) {
        assert(population->mutation_chance != 0);
        assert(individual < population->size);

        for (uint32_t current_gene = 0; current_gene < population->genes_per_individual; current_gene++) {
            if (RANDOM < population->mutation_chance) {
                // YEEEES. Generate new gene
                double new_gene = population->lower_gene_limit + (RANDOM * (population->upper_gene_limit - population->lower_gene_limit));
                population->individuals[individual].genes[current_gene] = new_gene;
                population->individuals[individual].mutated = true;
            }
        }
    }

// Crossover - if chance > crossover rate
void crossover(
    Chromossome a;
)

void genetic_algorithm(
    Population* population, 
    double (*fitness_function)(double *genes, uint32_t genes_per_individual)) {
        uint32_t improvements = 0;
        
        for (uint32_t individual = 0; individual < population->size; individual++) {
            
            // Mutation
            mutation(population, individual);

            // Crossover

        }

        population->current_generation++;
    }