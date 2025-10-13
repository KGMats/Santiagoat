#include <stdlib.h>
#include <stdio.h>
#include "Algorithms.h"

#include <string.h>

#include "Graph.h"
#include "Solutions.h"
#include "Stack.h"


// Funcao utilizada para verificar se o numero de vizinhos ativos
// é suficiente para ativar o nó que foi passado como argumento
bool activationFunction(Node *node)
{
    return (node->n_neighbors / node->n_active_neighbors) <= 2;
}




// Função que percorre o grafo e verifica quais nós satisfazem a função
// de ativação. Itera até que não hajam mais nós que possam ser ativados.
int propagate(const Graph* graph) {
    int changed = 0;

    for (int i = 0; i < graph->n_nodes; i++) {
        Node* n1 = graph->nodes[i];
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


// Funçao que verifica se um dado conjunto de nós (activeNodeIDs)
// é capaz de ativar todo o grafo
bool runTest(Graph* graph, uint64_t *activeNodeIDs, uint64_t n_ids) {
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


// Função auxiliar do merge sort
void merge(tuple *arr, uint64_t left, uint64_t mid, uint64_t right) {
    uint64_t n1 = mid - left + 1;
    uint64_t n2 = right - mid;

    tuple *L = malloc(n1 * sizeof(tuple));
    tuple *R = malloc(n2 * sizeof(tuple));

    for (uint64_t i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (uint64_t j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    uint64_t i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].degree <= R[j].degree)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(tuple *arr, uint64_t left, uint64_t right) {
    if (left < right) {
        uint64_t mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}


// Função similar à propagate, mas ao invez de verificar por todo o grafo
// em busca do nós para ativar, ativa apenas os nós dados por changed_nodes
// e iterativamente os nós que serão ativados por propagação destes.
uint64_t partialPropagate(const Graph *graph, const uint64_t n_changed, const uint64_t *changed_nodes) {
    if (n_changed == 0) return 0;
    char *inQueue = calloc((graph->n_nodes / 8) + 1, sizeof(char));

    uint64_t *queue = malloc(sizeof(uint64_t) * graph->n_nodes);
    uint64_t front = 0;
    uint64_t back = 0;

    for (uint64_t i = 0; i < n_changed; i++) {
        queue[back++] = changed_nodes[i];
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


// Função similar ao partialPropagate, mas aqui ao invez de ativarmos os nós
// dados por changed_nodes, desativamos eles e possivelmente os nós vizinhos
// de forma iterativa.
uint64_t partialReversePropagate(const Graph *graph, const uint64_t n_changed, const uint64_t *changed_nodes) {
    if (n_changed == 0) return 0;

    uint64_t *queue = malloc(sizeof(uint64_t) * graph->n_nodes);
    uint64_t front = 0;
    uint64_t back = 0;

    for (uint64_t i = 0; i < n_changed; i++) {
        queue[back++] = changed_nodes[i];
    }

    uint64_t new_count = 0;

    while (front < back) {
        const uint64_t nodeID = queue[front++];
        const Node *node = graph->nodes[nodeID];

        for (uint64_t i = 0; i < node->n_neighbors; i++) {
            Node *neighbor = node->neighbors[i];

            neighbor->n_active_neighbors--;
            if (!getNodeState(graph->active_nodes, neighbor->ID)) continue;


            if (neighbor->n_active_neighbors < neighbor->n_neighbors / 2.0f) {
                setNodeState(graph->active_nodes, neighbor->ID, 0);
                queue[back++] = neighbor->ID;

                new_count++;
            }
        }
    }

    free(queue);
    return new_count;
}


uint64_t min(uint64_t a, uint64_t b) {
    return (a < b) ? a : b;
}



// Algoritmo de tarjan para encontrar pontos de articulação em um grafo.
// Utilizado pela greedyHeuristics02 (a heuristica das pontes).
uint64_t* tarjan(Graph* graph) {
    const uint64_t n_nodes = graph->n_nodes;

    bool* visited = calloc(n_nodes, sizeof(bool));
    uint64_t* disc = malloc(n_nodes * sizeof(uint64_t));
    uint64_t* low = malloc(n_nodes * sizeof(uint64_t));
    uint64_t* parent = malloc(n_nodes * sizeof(uint64_t));
    bool* isAP = calloc(n_nodes, sizeof(bool));
    uint64_t* solutions = malloc(n_nodes * sizeof(uint64_t));
    uint64_t n_solutions = 0;
    uint64_t time = 0;

    for (uint64_t i = 0; i < n_nodes; i++) {
        parent[i] = UINT64_MAX;
        solutions[i] = UINT64_MAX;
    }

    if (visited == NULL || disc == NULL || low == NULL || parent == NULL || isAP == NULL || solutions == NULL) {
        printf("Deu erro aqui man\n");
        exit(0);
    }

    for (uint64_t i = 0; i < n_nodes; i++) {
        if (!visited[i]) {
            Node* start_node = graph->nodes[i];

            // Inicia o DFS iterativo
            dfs_AP(start_node, visited, disc, low, parent, isAP, graph, &time);

            // A REGRA DA RAIZ precisa ser verificada aqui, após o término do DFS
            // para um componente. Contamos quantos filhos a raiz teve.
            int root_children = 0;
            for (uint64_t j = 0; j < n_nodes; j++) {
                if (parent[j] == start_node->ID) {
                    root_children++;
                }
            }
            if (root_children > 1) {
                isAP[start_node->ID] = true;
            }
        }
    }

    for (uint64_t i = 0; i < n_nodes; i++) {
        if (isAP[i]) {
            solutions[n_solutions++] = i;
        }
    }

    free(visited);
    free(disc);
    free(low);
    free(parent);
    free(isAP);
    return solutions;
}


// Função auxiliar de dfs utilizada pelo Algoritmo de Tarjan
void dfs_AP(Node* start_node, bool visited[], uint64_t disc[], uint64_t low[], uint64_t parent[], bool isAP[], Graph* graph, uint64_t* time) {
    LinkedList* stack = NULL;

    // Cria e empilha o primeiro estado para o nó inicial
    StackFrame* start_frame = (StackFrame*)malloc(sizeof(StackFrame));
    start_frame->u = start_node;
    start_frame->neighbor_idx = 0;
    StackPush(&stack, start_frame);

    while (stack != NULL) {
        // Espia o topo da pilha sem remover
        StackFrame* current_frame = stack->frame;
        Node* u = current_frame->u;
        uint64_t u_id = u->ID;

        // Se é a primeira vez que processamos este nó, fazemos as ações de "pré-ordem"
        if (!visited[u_id]) {
            visited[u_id] = true;
            disc[u_id] = low[u_id] = ++(*time);
        }

        // Verifica se ainda há vizinhos para explorar a partir de 'u'
        if (current_frame->neighbor_idx < u->n_neighbors) {
            Node* v = u->neighbors[current_frame->neighbor_idx];
            uint64_t v_id = v->ID;

            // Incrementa o índice para a próxima vez que voltarmos a este frame
            current_frame->neighbor_idx++;

            // Ignora o pai
            if (v_id == parent[u_id]) {
                continue;
            }

            if (visited[v_id]) {
                // Se o vizinho já foi visitado, é um atalho (back-edge)
                low[u_id] = min(low[u_id], disc[v_id]);
            } else {
                // Se não foi visitado, definimos o pai e empilhamos um novo estado para 'v'
                parent[v_id] = u_id;

                StackFrame* new_frame = (StackFrame*)malloc(sizeof(StackFrame));
                new_frame->u = v;
                new_frame->neighbor_idx = 0;
                StackPush(&stack, new_frame);
            }
        } else {
            // Se todos os vizinhos de 'u' já foram processados, simulamos a "volta" da recursão
            uint64_t p_id = parent[u_id];

            // Se 'u' não for a raiz de uma árvore DFS, ele atualiza o low-link de seu pai
            if (p_id != UINT64_MAX) {
                low[p_id] = min(low[p_id], low[u_id]);

                // E o pai verifica se é um ponto de articulação por causa de 'u'
                if (low[u_id] >= disc[p_id]) {
                    isAP[p_id] = true;
                }
            }

            // Remove o estado de 'u' da pilha e libera sua memória
            StackFrame* done_frame = StackPop(&stack);
            free(done_frame);
        }
    }
}
