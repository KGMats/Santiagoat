#include <stdio.h>
#include <time.h>

#include "Algorithms.h"
#include "Solutions.h"
#include "Graph.h"


int main(const int argc, char** argv) {
    const char *filename = "../graphs/facebook_combined.txt";

    srand(time(NULL));

    if (argc > 1) {
         filename = argv[1];
    }
    Graph* graph = createGraphFromFilename(filename);

    testLocalSearch(graph, bridgeHeuristic, hillClimbSimple);
    // testHeuristics(graph, bridgeHeuristic);
    freeGraph(graph);
    return 0;
}
