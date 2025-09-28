#include <stdio.h>
#include <stdlib.h>

#include "Solutions.h"
#include "Algorithms.h"


bool greedyHeuristics01(Graph *graph, uint64_t initialActiveNodes) {
    tuple* list = malloc(sizeof(tuple) * graph->n_nodes);
    for (int i = 0; i < graph->n_nodes; i++) {
        list[i].ID = i;
        list[i].degree = graph->nodes[i]->n_neighbors;
    }

    mergeSort(list, 0, graph->n_nodes - 1);

    uint64_t *activeNodes = malloc(sizeof(uint64_t) * initialActiveNodes);

    for (int i = 0; i < initialActiveNodes; i++) {
        activeNodes[i] = list[graph->n_nodes - i - 1].ID;
    }

    return runTest(graph, activeNodes, initialActiveNodes);

}


void testHeuristics(Graph* graph, bool heuristicFunction(Graph*, uint64_t)) {
    uint64_t low = 1;
    uint64_t high = graph->n_nodes;
    uint64_t best = 0;

    while (low <= high) {
        uint64_t mid = (low + high) / 2;
        deactivateAll(graph);

        if (heuristicFunction(graph, mid)) {
            best = mid;
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }

    if (best != 0) {
        printf("%lu e a resposta\n", best);
        return;

    }

    printf("Nao existe solucao\n");

}
