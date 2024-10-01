#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdint.h>

typedef uint32_t vertex_t;

typedef struct _adj_list_t {
    vertex_t vertex;
    uint32_t weight;
    struct _adj_list_t *next;
} adj_list_t;

typedef adj_list_t node_t;

typedef struct _graph_t {
    uint32_t size;
    adj_list_t **graph;
} graph_t;

graph_t *graph_new(unsigned int num_vert);
//@ensures \result != NULL

void graph_free(graph_t *graph);
//@requires graph != NULL

uint32_t graph_size(graph_t *graph);
//@requires graph != NULL

bool graph_hasedge(graph_t *graph, vertex_t v, vertex_t w);
//@requires graph != NULL
//@requires v < graph_size(graph) && w < graph_size(graph)

void graph_addedge(graph_t *graph, vertex_t v, vertex_t w, uint32_t weight);
//@requires G != NULL
//@requires v < graph_size(G) && w < graph_size(graph)
//@requires v != w && !graph_hasedge(G, v, w)

node_t *graph_get_neighbors(graph_t *graph, vertex_t v);
//@requires graph != NULL && v < graph_size(graph)
//@ensures \result != NULL

bool graph_hasmore_neighbors(node_t *node);
//@requires node != NULL

node_t *graph_next_neighbor(node_t *node);
//@requires node != NULL
//@requires graph_hasmore_neighbors(node)

void graph_print(graph_t *graph);
//@requires queue != NULL

#endif /* __GRAPH_H__ */