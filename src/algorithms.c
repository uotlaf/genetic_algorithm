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

    for (uint32_t current_individual = 0; current_individual < population->size; current_individual++)
    {
        // Allocation phase
        population->individuals[current_individual].genes = malloc(sizeof(double) * population->genes_per_individual);
        assert(population->individuals[current_individual].genes);

        // Default values
        population->individuals[current_individual].selections = 0;
        population->individuals[current_individual].old_selections = 0;

        // Populate genes
        for (uint32_t current_gene = 0; current_gene < population->genes_per_individual; current_gene++)
        {
            // Generate random value from lower limit
            double random_value = population->lower_gene_limit + (RANDOM * (population->upper_gene_limit - population->lower_gene_limit));
            population->individuals[current_individual].genes[current_gene] = random_value;
        }

        // Calculate fitness function
        double response_fitness = fitness_function(population->individuals[current_individual].genes, population->genes_per_individual);
        population->individuals[current_individual].fitness = response_fitness;
        population->individuals[current_individual].old_fitness = response_fitness;

        fitness_sum += response_fitness;
    }

    population->sum_fitness = fitness_sum;
    population->current_generation = 0;
}

int fixOutOfBoundValues(double *xr, double linf, double lsup)
{
    if ((*xr) > lsup)
        *xr = lsup - RANDOM * ((*xr) - lsup) / ((*xr) - linf);
    else if ((*xr) < linf)
        *xr = linf + RANDOM * (linf - (*xr)) / (lsup - (*xr));
    return (1);
}

// Mutation - If chance > mutation rate,
void mutation(
    Population *population,
    uint64_t individual)
{
    assert(population);
    assert(population->mutation_chance != 0);
    assert(individual < population->size);

    for (uint32_t current_gene = 0; current_gene < population->genes_per_individual; current_gene++)
    {
        if (RANDOM < population->mutation_chance)
        {
            // YEEEES. Generate new gene
            double new_gene = population->lower_gene_limit + (RANDOM * (population->upper_gene_limit - population->lower_gene_limit));
            population->individuals[individual].genes[current_gene] = new_gene;
            population->individuals[individual].mutated = true;
            fixOutOfBoundValues(&(population->individuals[individual].genes[current_gene]), population->lower_gene_limit, population->upper_gene_limit);
        }
    }
}

// Crossover - if chance > crossover rate
void crossover(
    Population *population,
    uint64_t father, uint64_t mother, uint64_t son, float alpha)
{
    assert(population);
    assert(population->crossover_chance != 0);
    assert(father < population->size);
    assert(mother < population->size);
    assert(son < population->size);

    for (uint32_t current_gene = 0; current_gene < population->genes_per_individual; current_gene++)
    {
        double random_value = (-alpha) + (RANDOM) * ((1 + alpha) - (-alpha));
        population->individuals[son].genes[current_gene] =
            population->individuals[father].genes[current_gene] +
            random_value * (population->individuals[mother].genes[current_gene] +
                            population->individuals[father].genes[current_gene]);
        fixOutOfBoundValues(&(population->individuals[son].genes[current_gene]), population->lower_gene_limit, population->upper_gene_limit);
    }
}

void genetic_algorithm(
    Population *population,
    double (*fitness_function)(double *genes, uint32_t genes_per_individual))
{
    do
    {
        #pragma omp for
        for (uint32_t individual = 0; individual < population->size; individual++)
        {
            // Crossover chance
            if (RANDOM < population->mutation_chance)
            {
                uint32_t father = population->individuals[individual].genes[rand() % population->genes_per_individual];
                uint32_t mother = population->individuals[individual].genes[rand() % population->genes_per_individual];

                if (father != mother)
                {
                    crossover(
                        population, father, mother, individual,
                        XALPHA * (1.0 - (double)population->current_generation / population->max_generation));

                    // Recalculate fitness
                    population->individuals[individual].fitness = fitness_function(population->individuals[individual].genes, population->genes_per_individual);

                    // Mutation
                    mutation(population, individual);

                    // Fitness verification
                    if (population->individuals[individual].fitness < population->individuals[individual].old_fitness)
                    {
                        population->individuals[individual].selections++;
                    }
                }
            }
        }

        // Fitness update post-crossover
        #pragma omp for
        for (uint32_t individual = 0; individual < population->size; individual++)
        {
            // If individual is selected, maintain
            if (population->individuals[individual].selections > population->individuals[individual].old_selections)
            {
                population->individuals[individual].old_selections = population->individuals[individual].selections;
                population->individuals[individual].old_fitness = population->individuals[individual].fitness;
                population->individuals[individual].mutated = false;
            }
            else
            {
                // If not, generate a new individual
                population->individuals[individual].selections = 0;
                population->individuals[individual].old_selections = 0;

                // Repopulate genes
                for (uint32_t current_gene = 0; current_gene < population->genes_per_individual; current_gene++)
                {
                    // Generate random value from lower limit
                    double random_value = population->lower_gene_limit + (RANDOM * (population->upper_gene_limit - population->lower_gene_limit));
                    population->individuals[individual].genes[current_gene] = random_value;
                }

                // Recaulculate fitness function
                double response_fitness = fitness_function(population->individuals[individual].genes, population->genes_per_individual);
                population->individuals[individual].fitness = response_fitness;
                population->individuals[individual].old_fitness = response_fitness;
            }
        }

        population->current_generation++;
    } while (population->current_generation < population->max_generation);
}