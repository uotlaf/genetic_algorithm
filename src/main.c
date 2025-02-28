#include <algorithms.h>
#include <genectics.h>
#include <fitness.h>
#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    Population* population = malloc(sizeof(Population));
    clock_t clock_start, clock_end;

    assert(population);
    srand((unsigned) time(0));

    // Default values
    population->size                 = DEFAULT_POPULATION_SIZE;
    population->genes_per_individual = DEFAULT_GENES_PER_INDIVIDUAL;
    population->lower_gene_limit     = DEFAULT_LOWER_GENE_LIMIT;
    population->upper_gene_limit     = DEFAULT_UPPER_GENE_LIMIT;
    population->mutation_chance      = DEFAULT_MUTATION_CHANCE_PERCENT;
    population->crossover_chance     = DEFAULT_CROSSOVER_CHANCE_PERCENT;
    population->max_generation       = DEFAULT_MAX_GENERATIONS;

    printf("Pré-boot:\n");
    printf("\tNúmero máximo de threads: %d\n", omp_get_max_threads());
    printf("\tTamanho da população: %ld\n", population->size);
    printf("\tGenes por indivíduo: %d\n", population->genes_per_individual);
    printf("\tValor mínimo dos genes: %f\n", population->lower_gene_limit);
    printf("\tValor máximo dos genes: %f\n", population->upper_gene_limit);
    printf("\tChance de mutação: %d%%\n", population->mutation_chance);
    printf("\tChance de cruzamento: %d%%\n", population->crossover_chance);
    printf("\tLimite de gerações: %d\n", population->max_generation);

    printf("Dica: Utilize a variável de ambiente OMP_NUM_THREADS para configurar o número máximo de threads\n");
    
    printf("Boot:\n");
    printf("Alocando a população...");
    fflush(stdout);
    populate(population, fitness_functions[0]);
    printf("Pronto\n");

    // Para cada algoritmo
    // Para a quantidade máxima de gerações
    for (uint32_t current_fit_function = 0; current_fit_function < sizeof(fitness_functions) / sizeof(fitness_functions[0]); current_fit_function++)
    {
        printf("Executando o algoritmo %d...", current_fit_function);
        fflush(stdout);
        genetic_algorithm(population, fitness_functions[current_fit_function]);
        printf("Pronto!\n");
    }


    printf("Pressione enter pra continuar");
    getchar();
    return 0;
}