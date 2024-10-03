#ifndef __KRUSKAL_H__
#define __KRUSKAL_H__

#include "graph_search.h"
#include "min_heap.h"

typedef struct _graph_edges_t {
    uint32_t weight;
    vertex_t u;
    vertex_t v;
} graph_edge_t;

graph_t *spanning_tree(graph_t *graph);

void sort_edges(graph_t *graph, bool **mark, min_heap_t *min_heap);

graph_t *min_span_tree(graph_t *graph);

#endif /* __KRUSKAL_H__ */