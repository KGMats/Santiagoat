#include "Solutions.h"
#include "Graph.h"


int main(void) {
    const char *filename = "../soc-LiveJournal1.txt";
    Graph* graph = createGraphFromFilename(filename);
    testHeuristics(graph, greedyHeuristics01);

    //printf("%s", greedyHeuristics01(graph, 4847237) ? "Deus e bom" : "Diabo naum presta");
    return 0;
}