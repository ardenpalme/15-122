#ifndef __GRAPH_SEARCH_H__
#define __GRAPH_SEARCH_H__

#include "graph.h"
#include "queue.h"

node_t *dfs_search(graph_t *graph, bool *mark, vertex_t start_vert, vertex_t target_vert);
//@requires graph != NULL
//@requires start_vert < graph_size(graph) && trarget_vert < graph_size(graph)

queue_t *bfs_search(graph_t *graph, bool *mark, vertex_t start_vert, vertex_t target_vert);
//@requires graph != NULL

#endif /* __GRAPH_SEARCH_H__ */