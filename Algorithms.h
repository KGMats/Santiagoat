#ifndef SEMNOME_ALGORITHMS_H
#define SEMNOME_ALGORITHMS_H

#include "Graph.h"
#include "Solutions.h"
#include <stdlib.h>

int propagate(const Graph *graph);
uint64_t partialPropagate(const Graph *graph, uint64_t n_changed, const uint64_t *changed_nodes);
uint64_t partialReversePropagate(const Graph *graph, uint64_t n_changed, const uint64_t *changed_nodes);
bool runTest(Graph *graph, uint64_t *activeNodeIDS, uint64_t n_ids);
void printBits(size_t size, void const * ptr);
void mergeSort(tuple *arr, uint64_t left, uint64_t right);
uint64_t* tarjan(Graph *graph);
void dfs_AP(Node* startNode, bool visited[], uint64_t disc[], uint64_t low[], uint64_t parent[], bool isAP[], Graph* graph, uint64_t* time);

#endif //SEMNOME_ALGORITHMS_H
