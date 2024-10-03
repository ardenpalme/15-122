#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "graph_search.h"
#include "kruskal.h"
#include "min_heap.h"

void print_func1(const void *data) {
    node_t *node = (node_t *)data;
    printf("%d", node->vertex);
}

priority_t priority_func2(heap_elem_t elem) {
    uint32_t *val = (uint32_t *)elem;
    return *val;
}

void print_func2(heap_elem_t elem) {
    uint32_t *val = (uint32_t *)elem;
    printf("%d", *val);
}

void heap_add_ele(min_heap_t *min_heap, uint32_t val) {
    uint32_t *x = malloc(4);
    *x = val;
    min_heap_add(min_heap, (void*)x, priority_func2);
}

void heap_rem_ele(min_heap_t *min_heap) {
    heap_elem_t elem = min_heap_rem(min_heap, priority_func2);
    //printf("ret -> [%d]\n", *(int*)ret);

    graph_edge_t *edge = (graph_edge_t  *)elem;
    printf("ret -> (%d, %d, %d)\n", edge->u, edge->v, edge->weight);
}

void edge_print_func(heap_elem_t elem) {
    graph_edge_t *edge = (graph_edge_t  *)elem;
    printf("%d", edge->weight);
}

int main(int argc, char *argv[])
{
    graph_t *graph = graph_new(5);
    graph_addedge(graph,0,1,9); // A - B
    graph_addedge(graph,0,4,1); // A - E
    graph_addedge(graph,1,4,1); // B - E
    graph_addedge(graph,1,2,4); // B - C
    graph_addedge(graph,4,2,15); // E - C
    graph_addedge(graph,2,3,11); // D - C

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

    sort_edges(graph);
    */

    /*
    min_heap_t *min_heap = (min_heap_t*)calloc(1,sizeof(min_heap_t));
    heap_add_ele(min_heap, 2);
    heap_add_ele(min_heap, 3);
    heap_add_ele(min_heap, 4);
    min_heap_print(min_heap, print_func2);

    heap_add_ele(min_heap, 5);
    heap_add_ele(min_heap, 6);
    heap_add_ele(min_heap, 7);
    heap_add_ele(min_heap, 8);
    min_heap_print(min_heap, print_func2);

    heap_add_ele(min_heap, 1);
    min_heap_print(min_heap, print_func2);

    heap_rem_ele(min_heap);
    heap_rem_ele(min_heap);
    heap_rem_ele(min_heap);
    min_heap_print(min_heap, print_func2);

    heap_add_ele(min_heap, 10);
    heap_add_ele(min_heap, 11);
    heap_add_ele(min_heap, 12);
    heap_add_ele(min_heap, 13);
    min_heap_print(min_heap, print_func2);

    heap_rem_ele(min_heap);
    heap_rem_ele(min_heap);
    min_heap_print(min_heap, print_func2);
    */

    min_heap_t *min_heap = (min_heap_t*)calloc(1,sizeof(min_heap_t));
    bool **mark = malloc(graph->size * sizeof(bool *));
    for (int i = 0; i < graph->size; ++i) {
        mark[i] = malloc(graph->size * sizeof(bool));
    }

    sort_edges(graph, mark, min_heap);
    while(min_heap->root) {
        heap_rem_ele(min_heap);
        //min_heap_print(min_heap, edge_print_func);
        //printf("\n");
    }
    
    return 0;
}