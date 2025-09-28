#ifndef SEMNOME_ALGORITHMS_H
#define SEMNOME_ALGORITHMS_H

#include "Graph.h"
#include "Solutions.h"


int propagate(const Graph *graph, int iteration);
bool runTest(Graph *graph, uint64_t *activeNodeIDS, uint64_t n_ids);
void printBits(size_t size, void const * ptr);
void mergeSort(tuple *arr, uint64_t left, uint64_t right);

#endif //SEMNOME_ALGORITHMS_H