#include "Algorithms.h"
#include "Heuristics/MaximumDegreeHeuristic.h"
#include "Solutions.h"
#include "Graph.h"


int main(const int argc, char** argv) {
    const char *filename = "../graphs/facebook_combined.txt";

    if (argc > 1) {
         filename = argv[1];
    }
    Graph* graph = createGraphFromFilename(filename);
    testLocalSearch(graph, maximumDegreeHeuristic, hillClimbSimple);

    freeGraph(graph);
    return 0;
}
