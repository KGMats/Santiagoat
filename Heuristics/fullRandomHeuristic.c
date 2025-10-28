//
// Created by ra173048 on 10/27/25.
//

#include "fullRandomHeuristic.h"

#include <stdlib.h>

#include "Algorithms.h"

bool fullRandomHeuristic(const Graph *graph, uint64_t *result, uint64_t initialActiveNodes) {
    for (uint64_t i = 0; i < initialActiveNodes; i++) {
        result[i] = rand() % graph->n_nodes;
    }

    partialPropagate(graph, initialActiveNodes, result);
    return (graph->n_nodes == countActiveNodes(graph));
}
