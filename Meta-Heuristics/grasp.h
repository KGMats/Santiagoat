//
// Created by ra173048 on 10/24/25.
//

#ifndef GRAFOS_GRASP_H
#define GRAFOS_GRASP_H
#include "Graph.h"


void GRASP(Graph* graph, uint64_t initialActiveNodes,
        const unsigned int max_iterations,
        int seed, bool randomGreedyFunction(Graph*, uint64_t),
        uint64_t localSearchFunction(Graph*, uint64_t));

#endif //GRAFOS_GRASP_H