//
// Created by Elon Pereira Neto on 24/10/25.
//

#include <stdlib.h>
#include <time.h>

#include "Utils.h"
#include "Algorithms.h"
#include "hillClimbLocalSearch.h"



// Função de busca local utilizando a estratégia "tira dois, põe um"
uint64_t hillClimbSimple(const Graph* graph, const uint64_t nActiveNodes) {
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
                const uint64_t r = rand() % graph->n_nodes;
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
            const bool sucesso = partialPropagate(graph, 1, &toActivate) == graph->n_nodes;

            if (sucesso) {
                melhorAtivos--;
                melhorou = true;
                break;
            }


            // Se não for, restaura os estados anteriores dos nós antes da próxima iteração
            setNodeState(graph->active_nodes, toActivate, false);
            setNodeState(graph->active_nodes, toRemove[0], true);
            setNodeState(graph->active_nodes, toRemove[1], true);
        }
    }

    free(toRemove);

    return melhorAtivos;
}