#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph_search.h"

int main(int argc, char *argv[])
{
    graph_t *graph = graph_new(5);
    graph_addedge(graph,0,1); // A - B
    graph_addedge(graph,0,4); // A - E
    graph_addedge(graph,1,4); // B - E
    graph_addedge(graph,1,2); // B - C
    graph_addedge(graph,4,2); // E - C
    graph_addedge(graph,2,3); // D - C

    graph_print(graph);

    bool *mark = calloc(5, sizeof(bool));
    (void)bfs_search(graph, mark, 0, 2);

    return 0;
}