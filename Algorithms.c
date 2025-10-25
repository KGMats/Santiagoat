#include <stdlib.h>
#include <stdio.h>

#include "Graph.h"
#include "Solutions.h"
#include "Stack.h"
#include "Algorithms.h"

#include "Utils.h"


// Funcao utilizada para verificar se o número de vizinhos ativos
// é suficiente para ativar o nó passado como argumento
bool activationFunction(const Node *node)
{
    if (node->n_active_neighbors == 0)
    {
        return false;
    }
    return (node->n_neighbors / node->n_active_neighbors) <= 2;
}



// Função que percorre o grafo e verifica quais nós satisfazem a função
// de ativação. Itera até que não hajam mais nós que possam ser ativados.
int propagate(const Graph* graph) {
    int changed = 0;

    for (int i = 0; i < graph->n_nodes; i++) {
        const Node* n1 = graph->nodes[i];
        if (getNodeState(graph->active_nodes, i)) {
            // Se o nó já está ativado, não precisamos verificar se ele precisa ser ativado.
            continue;
        }


        if (activationFunction(n1)) {
            setNodeState(graph->active_nodes, i, 1);
            changed = 1;

            for (uint64_t j = 0; j < n1->n_neighbors; j++) {
                n1->neighbors[j]->n_active_neighbors++;
            }
        }
    }

    return changed;
}



// Função que verifica se um dado conjunto de nós (activeNodeIDs)
// é capaz de ativar inteiramente o grafo
bool runTest(const Graph* graph, const uint64_t *activeNodeIDs, const uint64_t n_ids) {
    int iteration = 0;
    activateFromIDArray(graph, activeNodeIDs, n_ids);

    do {
        iteration++;
    } while (propagate(graph));

    uint64_t counter = 0;
    for (uint64_t i = 0; i < graph->n_nodes; i++) {
        if (getNodeState(graph->active_nodes, i)) {
            counter++;
        }
    }

    return counter == graph->n_nodes;
}



// Função similar a propagate, mas ao invés de verificar o grafo inteiramente
// em busca dos nós para ativar, ativa apenas os nós dados por changed_nodes
// e iterativamente os nós que serão ativados por propagação destes.
uint64_t partialPropagate(const Graph *graph, const uint64_t n_changed, const uint64_t *changed_nodes) {
    if (n_changed == 0) return 0;
    char *inQueue = calloc((graph->n_nodes / 8) + 1, sizeof(char));

    uint64_t *queue = malloc(sizeof(uint64_t) * graph->n_nodes);
    uint64_t front = 0;
    uint64_t back = 0;

    for (uint64_t i = 0; i < n_changed; i++) {
        queue[back++] = changed_nodes[i];
        setNodeState(graph->active_nodes, changed_nodes[i], 1);
        setNodeState(inQueue, changed_nodes[i], 1);
    }

    uint64_t new_count = 0;

    while (front < back) {
        const uint64_t nodeID = queue[front++];
        const Node *node = graph->nodes[nodeID];

        for (uint64_t i = 0; i < node->n_neighbors; i++) {
            Node *neighbor = node->neighbors[i];

            neighbor->n_active_neighbors++;
            if (getNodeState(graph->active_nodes, neighbor->ID)) continue;


            if (activationFunction(neighbor)) {
                setNodeState(graph->active_nodes, neighbor->ID, 1);
                if (!getNodeState(inQueue, neighbor->ID)) {
                    queue[back++] = neighbor->ID;
                    setNodeState(inQueue, neighbor->ID, 1);
                }
                new_count++;
            }
        }
    }

    free(queue);
    return new_count;
}


// Função similar ao partialPropagate, mas aqui ao invés de ativarmos os nós
// dados por changed_nodes, desativamos eles e possivelmente os nós vizinhos
// de forma iterativa.
uint64_t partialReversePropagate(const Graph *graph, const uint64_t n_changed, const uint64_t *changed_nodes) {
    if (n_changed == 0) return 0;

    uint64_t *queue = malloc(sizeof(uint64_t) * graph->n_nodes);
    uint64_t front = 0;
    uint64_t back = 0;

    for (uint64_t i = 0; i < n_changed; i++) {
        queue[back++] = changed_nodes[i];
        setNodeState(graph->active_nodes, changed_nodes[i], 0);
    }

    uint64_t new_count = 0;

    while (front < back) {
        const uint64_t nodeID = queue[front++];
        const Node *node = graph->nodes[nodeID];

        for (uint64_t i = 0; i < node->n_neighbors; i++) {
            Node *neighbor = node->neighbors[i];

            neighbor->n_active_neighbors--;
            if (!getNodeState(graph->active_nodes, neighbor->ID)) continue;


            if (!activationFunction(neighbor)) {
                setNodeState(graph->active_nodes, neighbor->ID, 0);
                queue[back++] = neighbor->ID;

                new_count++;
            }
        }
    }

    free(queue);
    return new_count;
}



void testHeuristics(const Graph* graph, bool heuristicFunction(const Graph*, uint64_t)) {
    uint64_t low = 1;
    const uint64_t n_nodes = graph->n_nodes;
    uint64_t high = n_nodes;
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
        printf("%lu/%lu Nos foram necessarios. Isso e %.5lf%% do total\n", best, n_nodes, 100. * ((double) best / (double) n_nodes)); return;

    }

    printf("Nao existe solucao\n");
}


// Função que utiliza de uma heuristica para encontrar uma solução e, em seguida
// Utiliza uma função de busca local para tentar encontrar um vizinho melhor
// Que a solução previamente calculada pela heuristica.
void testLocalSearch(const Graph *graph, bool heuristicFunction(const Graph*, uint64_t), uint64_t localSearchFunction(const Graph* graph, uint64_t nActiveNodes)) {
    // Tentando encontrar uma solução inicial com a heuristica
    uint64_t low = 1;
    const uint64_t n_nodes = graph->n_nodes;
    uint64_t high = n_nodes;
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

    if (best == 0) {
        printf("A heuristica não encontrou solução, portanto não há como fazer a busca local");
        return;
    }

    printBits(graph->n_nodes, graph->active_nodes);
    const uint64_t bestLocal = localSearchFunction(graph, best);

    if (best != bestLocal)
    {
        printf("--------------------------------------------------\n");
        printf("Heuristica: %lu Nos ativos (%0.2f%% do total)\n", best, 100 *(float) best / graph->n_nodes);
        printf("Busca Local: %lu Nos ativos (%0.2f%% do total)\n", bestLocal, 100 * (float) bestLocal / graph->n_nodes);
        printf("Diferenca de %0.2f%% de redução\n", ((float) (best - bestLocal) / best) * 100);
        printf("--------------------------------------------------\n");
        return;
    }

    printf("A busca local não conseguiu melhorar o resultado da heuristica\n");
    printf("%lu Nos ativos (%0.2f%% do total)\n", best, 100 *(float) best / graph->n_nodes);
}
