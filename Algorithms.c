#include <stdlib.h>
#include <string.h>

#include "Algorithms.h"
#include "Graph.h"
#include "Solutions.h"


int propagate(const Graph* graph, int iteration) {
    char* copy = malloc((graph->n_nodes / 8) + 1);
    memcpy(copy, graph->active_nodes, (graph->n_nodes / 8) + 1);

    int changed = 0;
    for (int i = 0; i < graph->n_nodes; i++) {
        Node* n1 = graph->nodes[i];
        if (getNodeState(copy, i)) {
            continue;
        }

        int counter = 0;
        for (int j = 0; j < n1->n_neighbors; j++) {
            Node* n2 = n1->neighbors[j];
            if (getNodeState(copy, n2->ID)) {
                counter++;
            }
        }
        if ((float) counter >= (float) n1->n_neighbors / 2.0f) {
            setNodeState(graph->active_nodes, i, 1);
            changed = 1;
        }

    }
    free(copy);
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

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
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
