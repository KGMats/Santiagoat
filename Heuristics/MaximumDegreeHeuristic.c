//
// Created by Elon Pereira Neto on 24/10/25.
//

#include "Utils.h"
#include "Algorithms.h"
#include "MaximumDegreeHeuristic.h"

// Heuristica do número de Arestas
bool maximumDegreeHeuristic(const Graph *graph, const uint64_t initialActiveNodes) {
    tuple *list = orderedList(graph);

    uint64_t *activeNodes = malloc(sizeof(uint64_t) * initialActiveNodes);

    for (int i = 0; i < initialActiveNodes; i++) {
        // activeNodes[i] = list[graph->n_nodes - i - 1].ID;
        activeNodes[i] = list[i].ID;
    }

    free(list);
    return runTest(graph, activeNodes, initialActiveNodes);

}
