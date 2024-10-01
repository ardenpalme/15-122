#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "graph_search.h"
#include "kruskal.h"


void print_func1(const void *data)
{
    node_t *node = (node_t *)data;
    printf("%d", node->vertex);
}

int main(int argc, char *argv[])
{
    graph_t *graph = graph_new(5);
    graph_addedge(graph,0,1,3); // A - B
    graph_addedge(graph,0,4,2); // A - E
    graph_addedge(graph,1,4,2); // B - E
    graph_addedge(graph,1,2,3); // B - C
    graph_addedge(graph,4,2,2); // E - C
    graph_addedge(graph,2,3,3); // D - C

    graph_print(graph);
    printf("\n\n");

    /*
    bool *mark = calloc(5, sizeof(bool));
    queue_t *path = bfs_search(graph, mark, 0, 2);
    print_queue(path,print_func1);

    memset(mark, 0, 5 * sizeof(bool));
    (void)dfs_search(graph, mark, 0, 2);

    //graph_t *span_tree = spanning_tree(graph);
    //graph_print(span_tree);

    bfs_edges(graph);
    */

    sort_edges(graph);

    return 0;
}