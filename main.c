#include "Solutions.h"
#include "Graph.h"


int main(int argc, char** argv) {
    const char *filename = "../graphs/facebook_combined.txt";

    if (argc > 1) {
         filename = argv[1];
    }
    Graph* graph = createGraphFromFilename(filename);
    testLocalSearch(graph, greedyHeuristics01, hillClimbSimple);

    freeGraph(graph);
    return 0;
}
