#include "Solutions.h"
#include "Graph.h"


int main(int argc, char** argv) {
    const char *filename = "../graphs/soc-LiveJournal1.txt";

    if (argc > 1) {
         filename = argv[1];
    }
    Graph* graph = createGraphFromFilename(filename);
    testHeuristics(graph, greedyHeuristics01);

    freeGraph(graph);
    return 0;
}
