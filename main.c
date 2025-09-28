#include <stdio.h>

#include "Solutions.h"
#include "Graph.h"


int main(int argc, char** argv) {
    const char *filename = "../facebook_combined.txt";

    if (argc > 1) {
        filename = argv[1];
    }
    Graph* graph = createGraphFromFilename(filename);
    testHeuristics(graph, greedyHeuristics02);

    // printf("%s", greedyHeuristics01(graph, 4847237) ? "Deus e bom" : "Diabo naum presta");
    freeGraph(graph);

    return 0;
}