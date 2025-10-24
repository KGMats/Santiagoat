//
// Created by Elon Pereira Neto on 24/10/25.
//

#ifndef GRAFOS_TARJAN_H
#define GRAFOS_TARJAN_H

#include <stdint.h>
#include <stdbool.h>

#include "Graph.h"

uint64_t* tarjan(Graph* graph);
void dfs_AP(Node* startNode, bool visited[], uint64_t disc[], uint64_t low[], uint64_t parent[], bool isAP[], Graph* graph, uint64_t* time);


#endif //GRAFOS_TARJAN_H