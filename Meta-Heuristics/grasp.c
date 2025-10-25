#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "Graph.h"

uint64_t GRASP(Graph* graph, uint64_t initialActiveNodes,
        const unsigned int max_iterations,
        const int seed, const bool randomGreedyFunction(Graph*, uint64_t),
        const uint64_t localSearchFunction(Graph*, uint64_t))
{
        srand(seed);

        uint64_t best_solution = 0; // 0 indica que não tem solucao


        for (int i = 0; i < max_iterations; i++) {
                if (initialActiveNodes == 0) {
                        break;
                }

                bool is_solution = randomGreedyFunction(graph, initialActiveNodes);
                if (is_solution) {
                        best_solution = initialActiveNodes;
                        uint64_t solution = localSearchFunction(graph, initialActiveNodes);
                        best_solution = (solution > best_solution) ? solution : best_solution;
                        initialActiveNodes--;
                }

        }
        return best_solution;
}
