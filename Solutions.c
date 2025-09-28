#include <stdio.h>
#include <stdlib.h>

#include "Solutions.h"
#include "Algorithms.h"

// TODO: Implementar o partialPropagate() na heuristica01 para melhorar as escolhas dos nos iniciais, evitando
// Escolher nos que ja seriam ativos por propagacao.
tuple *orderedList(Graph *graph) {
    tuple* list = malloc(sizeof(tuple) * graph->n_nodes);
    for (int i = 0; i < graph->n_nodes; i++) {
        list[i].ID = i;
        list[i].degree = graph->nodes[i]->n_neighbors;
    }

    mergeSort(list, 0, graph->n_nodes - 1);

    return list;
}

// Heuristica do numero de Arestas
bool greedyHeuristics01(Graph *graph, uint64_t initialActiveNodes) {
    tuple *list = orderedList(graph);

    uint64_t *activeNodes = malloc(sizeof(uint64_t) * initialActiveNodes);

    for (int i = 0; i < initialActiveNodes; i++) {
        activeNodes[i] = list[graph->n_nodes - i - 1].ID;
    }

    free(list);
    return runTest(graph, activeNodes, initialActiveNodes);

}

// Algoritmo da "Ponte"
bool greedyHeuristics02(Graph *graph, uint64_t initialActiveNodes) {
    tuple *list = orderedList(graph);
    uint64_t *activeNodes = malloc(sizeof(uint64_t) * initialActiveNodes);
    uint64_t counter = 0;

    uint64_t *changedNodes = malloc(sizeof(uint64_t) * graph->n_nodes);
    uint64_t n_changed = 0;

    // Encontrando os candidatos a nos de "ponte"
    for (uint64_t i = 0; i < graph->n_nodes && counter < initialActiveNodes; i++) {
        uint64_t nodeID = list[i].ID;

        if (getNodeState(graph->active_nodes, nodeID)) continue;

        activeNodes[counter++] = nodeID;
        changedNodes[0] = nodeID;
        n_changed = 1;

        while (n_changed > 0) {
            n_changed = partialPropagate(graph, n_changed, changedNodes);
        }
    }

    // Completa o restante dos nos caso o initiaActivenodes > n de nos selecionados.
    uint64_t i = graph->n_nodes;
    while (i > 0 && counter < initialActiveNodes) {
        i--;
        uint64_t nodeID = list[i].ID;

        if (getNodeState(graph->active_nodes, nodeID)) continue;

        activeNodes[counter++] = nodeID;
        changedNodes[0] = nodeID;
        n_changed = 1;

        while (n_changed > 0) {
            n_changed = partialPropagate(graph, n_changed, changedNodes);
        }
    }

    // Contagem final de nós ativos
    uint64_t totalActiveNodes = 0;
    for (uint64_t i = 0; i < graph->n_nodes; i++) {
        if (getNodeState(graph->active_nodes, i)) totalActiveNodes++;
    }

    free(activeNodes);
    free(changedNodes);
    free(list);

    return totalActiveNodes == graph->n_nodes;
}



void testHeuristics(Graph* graph, bool heuristicFunction(Graph*, uint64_t)) {
    uint64_t low = 1;
    uint64_t high = graph->n_nodes;
    uint64_t best = 0;

    while (low <= high) {
        const uint64_t mid = (low + high) / 2;
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
        printf("%lu/%lu Nos foram necessarios. Isso e %.5lf%% do total\n", best, graph->n_nodes, 100. * best / graph->n_nodes);
        return;

    }

    printf("Nao existe solucao\n");

}
