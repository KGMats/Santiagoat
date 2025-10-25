//
// Created by Elon Pereira Neto on 24/10/25.
//

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include "Utils.h"
#include "Algorithms.h"
#include "hillClimbLocalSearch.h"


// Função de busca local utilizando a estratégia "tira dois, põe um"
uint64_t hillClimbSimple(const Graph* graph, uint64_t nActiveNodes) {
    const int MAX_ITERATIONS = 100000;

    uint64_t *toRemove = malloc(sizeof(uint64_t) * 2);
    uint64_t *toActivate = malloc(sizeof(uint64_t) * 1);
    uint64_t melhorAtivos = nActiveNodes;

    uint64_t *activeList = malloc(sizeof(uint64_t) * graph->n_nodes);
    uint64_t *inactiveList = malloc(sizeof(uint64_t) * graph->n_nodes);


    bool melhorou = true;

    while (melhorou) {
        melhorou = false;
        uint64_t active_index = 0;
        uint64_t inactive_index = 0;





        for (uint64_t i = 0; i < graph->n_nodes; i++) {
            bool state = getNodeState(graph->active_nodes, i);
            if (state) {
                activeList[active_index++] = i;
                continue;
            }
            inactiveList[inactive_index++] = i;
        }

        if (active_index < 2 || nActiveNodes <= 1) {
            // Nao tem como "Tirar dois e colocar um" se nao tiverem pelo menos dois ativos.
            break;
        }
        

        if (activeList == NULL || inactiveList == NULL) {
            perror("Erro na alocacao de memoria nas listas do Hill Climb");
            exit(EXIT_FAILURE);
        }


        for (int i = 0; i < MAX_ITERATIONS; ++i) {
            toRemove[0] = activeList[rand() % active_index];
            do {
                toRemove[1] = activeList[rand() % active_index];
            } while (toRemove[1] == toRemove[0]);

            if (inactive_index != 0) {
                *toActivate = inactiveList[rand() % active_index];
            }
            else {
                do {
                    *toActivate = activeList[rand() % active_index];
                } while (*toActivate == toRemove[1] || *toActivate == toRemove[0]);
            }

            printBits(graph->n_nodes, graph->active_nodes);
            partialReversePropagate(graph, 2, toRemove);
            printBits(graph->n_nodes, graph->active_nodes);
            nActiveNodes -= 1;


            // Testando se é uma solução válida


            printBits(graph->n_nodes, graph->active_nodes);
            const uint64_t newActiveNodes_propagated = partialPropagate(graph, 1, toActivate);
            printBits(graph->n_nodes, graph->active_nodes);
            const uint64_t totalAtivos = countActiveNodes(graph);

            printf("%ld de %ld. (Tentando com %ld nos)\n", totalAtivos, graph->n_nodes, nActiveNodes);
            printf("\n");

            if (totalAtivos == graph->n_nodes) {
                melhorAtivos = nActiveNodes;
                printf("MELHOROU\n");
                melhorou = true;
                break;
            }

            // Se não for, restaura os estados anteriores
            partialReversePropagate(graph, 1, toActivate);
            partialPropagate(graph, 2, toRemove);
            nActiveNodes += 1;
        }
    }

    free(toActivate);
    free(toRemove);
    free(activeList);
    free(inactiveList);


    return melhorAtivos;
}