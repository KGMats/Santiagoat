#include <stdlib.h>
#include "Algorithms.h"
#include "Graph.h"
#include "Solutions.h"


int propagate(const Graph* graph, int iteration) {
    int changed = 0;


    for (int i = 0; i < graph->n_nodes; i++) {
        Node* n1 = graph->nodes[i];
        if (getNodeState(graph->active_nodes, i)) {
            continue;
        }


        if (n1->n_active_neighbors >= n1->n_neighbors / 2.0f) {
            setNodeState(graph->active_nodes, i, 1);
            changed = 1;

            for (uint64_t j = 0; j < n1->n_neighbors; j++) {
                n1->neighbors[j]->n_active_neighbors++;
            }
        }
    }

    return changed;
}

bool runTest(Graph* graph, uint64_t *activeNodeIDs, uint64_t n_ids) {
    int iteration = 0;
    activateFromIDArray(graph, activeNodeIDs, n_ids);

    do {
        iteration++;
    } while (propagate(graph, iteration));

    uint64_t counter = 0;
    for (uint64_t i = 0; i < graph->n_nodes; i++) {
        if (getNodeState(graph->active_nodes, i)) {
            counter++;
        }
    }

    return counter == graph->n_nodes;
}




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


int partialPropagate(const Graph *graph, uint64_t n_changed, uint64_t *changed_nodes) {
    if (n_changed == 0) return 0;

    uint64_t *queue = malloc(sizeof(uint64_t) * graph->n_nodes);
    uint64_t front = 0;
    uint64_t back = 0;

    for (uint64_t i = 0; i < n_changed; i++) {
        queue[back++] = changed_nodes[i];
    }

    uint64_t new_count = 0;

    while (front < back) {
        uint64_t nodeID = queue[front++];
        Node *node = graph->nodes[nodeID];

        for (uint64_t i = 0; i < node->n_neighbors; i++) {
            Node *neighbor = node->neighbors[i];
            if (!neighbor) continue;

            if (getNodeState(graph->active_nodes, neighbor->ID)) continue;

            neighbor->n_active_neighbors++;

            if (neighbor->n_active_neighbors >= neighbor->n_neighbors / 2.0f) {
                setNodeState(graph->active_nodes, neighbor->ID, 1);
                queue[back++] = neighbor->ID;

                changed_nodes[new_count++] = neighbor->ID; // atualiza vetor de saída
            }
        }
    }

    free(queue);
    return new_count;
}
