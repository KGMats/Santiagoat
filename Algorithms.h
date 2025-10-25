#ifndef SEMNOME_ALGORITHMS_H
#define SEMNOME_ALGORITHMS_H

#include <stdlib.h>

#include "Graph.h"

int propagate(const Graph *graph);

uint64_t partialPropagate(const Graph *graph, uint64_t n_changed, const uint64_t *changed_nodes);
uint64_t partialReversePropagate(const Graph *graph, uint64_t n_changed, const uint64_t *changed_nodes);
bool runTest(const Graph *graph, const uint64_t *activeNodeIDs, uint64_t n_ids);
void testHeuristics(const Graph* graph, bool heuristicFunction(const Graph*, uint64_t));
void testLocalSearch(const Graph *graph, bool heuristicFunction(const Graph*, uint64_t), uint64_t localSearchFunction(const Graph* graph, uint64_t nActiveNodes));


#endif //SEMNOME_ALGORITHMS_H
