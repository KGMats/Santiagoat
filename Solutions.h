#ifndef SEMNOME_SOLUTIONS_H
#define SEMNOME_SOLUTIONS_H

#include <stdbool.h>
#include "Graph.h"


typedef struct {
    uint64_t ID;
    uint64_t degree;
} tuple;

bool greedyHeuristics01(Graph *graph, uint64_t initialActiveNodes);
void testHeuristics(Graph* graph, bool heuristicFunction(Graph*, uint64_t));

#endif //SEMNOME_SOLUTIONS_H