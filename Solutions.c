#include <stdio.h>
#include <stdlib.h>

#include "Solutions.h"

#include <time.h>

#include "Algorithms.h"

// TODO: Implementar o partialPropagate() na heuristica01 para melhorar as escolhas dos nos iniciais, evitando
// Escolher nos que ja seriam ativos por propagacao.

// TODO: Integrar a funcao de busca local com as heuristicas.



// Função auxiliar que ordena os grafos pelo seu grau
tuple *orderedList(const Graph *graph) {
    tuple *list = malloc(sizeof(tuple) * graph->n_nodes);
    for (int i = 0; i < graph->n_nodes; i++) {
        list[i].ID = i;
        list[i].degree = graph->nodes[i]->n_neighbors;
    }

    mergeSort(list, 0, graph->n_nodes - 1);

    return list;
}

tuple *orderedSetList(const Graph *graph, const uint64_t *IDsList) {
    const uint64_t n_nodes = graph->n_nodes;
    uint64_t n_ids = 0;
    tuple *list = calloc(n_nodes,sizeof(tuple));
    for (uint64_t i = 0; IDsList[i] != UINT64_MAX; i++) {
        list[i].ID = IDsList[i];
        list[i].degree = graph->nodes[IDsList[i]]->n_neighbors;
        n_ids++;
    }

    uint64_t id_search = 0;
    for (uint64_t i = 0; i < n_nodes; i++) {
        bool isInList = false;
        for (uint64_t j = 0; j < n_ids; j++) if (i == IDsList[j]) isInList = true;

        if (!isInList) {
            list[id_search + n_ids].ID = i;
            list[id_search + n_ids].degree = graph->nodes[i]->n_neighbors;
            id_search++;
        }
    }

    mergeSort(list, 0, n_ids - 1);
    mergeSort(list, n_ids, n_nodes - 1);
    return list;
}


// Heuristica do número de Arestas
bool greedyHeuristics01(Graph *graph, uint64_t initialActiveNodes) {
    tuple *list = orderedList(graph);

    uint64_t *activeNodes = malloc(sizeof(uint64_t) * initialActiveNodes);

    for (int i = 0; i < initialActiveNodes; i++) {
        activeNodes[i] = list[graph->n_nodes - i - 1].ID;
    }

    free(list);
    return runTest(graph, activeNodes, initialActiveNodes);

}


// Função de busca local utilizando a estratégia "tira dois, põe um"
uint64_t hillClimbSimple(Graph* graph, uint64_t nActiveNodes) {
    const int MAX_ITERATIONS = 1000;
    srand(time(NULL));

    uint64_t* toRemove = malloc(sizeof(uint64_t) * 2);
    uint64_t melhorAtivos = nActiveNodes;
    bool melhorou = true;

    while (melhorou) {
        melhorou = false;

        for (int i = 0; i < MAX_ITERATIONS; ++i) {
            // Escolhendo os nos para remover
            int count = 0;
            while (count < 2) {
                uint64_t r = rand() % graph->n_nodes;
                if (getNodeState(graph->active_nodes, r)) {
                    toRemove[count++] = r;
                }
            }

            // Escolhendo o nó para ativar
            uint64_t toActivate;
            do {
                toActivate = rand() % graph->n_nodes;
            } while (getNodeState(graph->active_nodes, toActivate));

            // Fazendo as devidas modificações nos estados dos nós
            setNodeState(graph->active_nodes, toRemove[0], false);
            setNodeState(graph->active_nodes, toRemove[1], false);
            setNodeState(graph->active_nodes, toActivate, true);

            // Testando se é uma solução válida
            bool sucesso = partialPropagate(graph, 1, &toActivate);

            if (sucesso) {
                melhorAtivos--;
                melhorou = true;
                printf("Nova melhor solução: %lu nós ativos\n", melhorAtivos);
                break;
            }


            // Se não for, restaura os estados anteriores dos nós antes da próxima iteração
            setNodeState(graph->active_nodes, toActivate, false);
            setNodeState(graph->active_nodes, toRemove[0], true);
            setNodeState(graph->active_nodes, toRemove[1], true);
        }
    }
    return melhorAtivos;
}




// Heuristica das pontes.
bool greedyHeuristics02(Graph *graph, uint64_t initialActiveNodes) {
    uint64_t *activeNodes = malloc(sizeof(uint64_t) * initialActiveNodes);
    uint64_t counter = 0;

    uint64_t *changedNodes = malloc(sizeof(uint64_t) * graph->n_nodes);
    uint64_t n_changed = 0;

    // Encontrando os candidatos a nos "Ponto de articulação"
    uint64_t *tarjanSolutions = tarjan(graph);
    tuple *list = orderedSetList(graph, tarjanSolutions);
    uint64_t n_ids = 0;
    for (uint64_t i = 0; tarjanSolutions[i] != UINT64_MAX; i++) n_ids++;

    for (uint64_t i = n_ids - 1; i > 0 && counter < initialActiveNodes; i--) {
        const uint64_t nodeID = list[i].ID;

        if (getNodeState(graph->active_nodes, nodeID)) continue;

        activeNodes[counter++] = nodeID;
        changedNodes[n_changed++] = nodeID;
    }

    // Completa o restante dos nos caso o initialActive nodes > n de nos selecionados.
    uint64_t i = graph->n_nodes;
    while (i > n_ids && counter < initialActiveNodes) {
        i--;
        const uint64_t nodeID = list[i].ID;

        if (getNodeState(graph->active_nodes, nodeID)) continue;

        activeNodes[counter++] = nodeID;
        changedNodes[n_changed++] = nodeID;

    }

    free(changedNodes);
    free(list);
    free(tarjanSolutions);

    return runTest(graph, activeNodes, initialActiveNodes);
}


void testHeuristics(Graph* graph, bool heuristicFunction(Graph*, uint64_t)) {
    uint64_t low = 1;
    uint64_t n_nodes = graph->n_nodes;
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
        printf("%lu/%lu Nos foram necessarios. Isso e %.5lf%% do total\n", best, n_nodes, 100. * ((double) best / (double) n_nodes));
        return;

    }

    printf("Nao existe solucao\n");
}


// Função que utiliza de uma heuristica para encontrar uma solução e, em seguida
// Utiliza uma função de busca local para tentar encontrar um vizinho melhor
// Que a solução previamente calculada pela heuristica.
void testLocalSearch(Graph *graph,  bool heuristicFunction(Graph*, uint64_t), bool localSearchFunction(Graph* graph, uint64_t nActiveNodes))
{
    // Tentando encontrar uma solução inicial com a heuristica
    uint64_t low = 1;
    uint64_t n_nodes = graph->n_nodes;
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
    
    int bestLocal = localSearchFunction(graph, best);
    if (best != bestLocal)
    {
        printf("A busca local precisou de %d nos, contra %d da heuristica", bestLocal, best);
        return;
    }
    printf("A busca local não conseguiu melhorar o resultado da heuristica").
    return;

}