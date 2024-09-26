#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "graph.h"

graph_t *graph_new(unsigned int num_vert)
//@ensures \result != NULL
{
    graph_t *graph = (graph_t *)calloc(1,sizeof(graph_t));
    graph->graph = (adj_list_t *)calloc(num_vert, sizeof(adj_list_t*));
    graph->size = num_vert;
}

void graph_free(graph_t *graph)
//@requires graph != NULL
{
    adj_list_t *node;
    for(int i=0; i<graph->size; i++)
    {
        node = graph->graph[i];
        adj_list_t *next_node;
        while(node != NULL){
            next_node = node->next;
            free(node);
            node = next_node;
        }
    }
}

uint32_t graph_size(graph_t *graph)
//@requires graph != NULL
{
    return graph->size;
}

bool graph_hasedge(graph_t *graph, vertex_t v, vertex_t w)
//@requires graph != NULL
//@requires v < graph_size(graph) && w < graph_size(graph)
{
    bool w_in_v = false;
    adj_list_t *node = graph->graph[v];
    while(node != NULL)
    {
        if(node->vertex == w){
            w_in_v = true;
            break;
        }
        node = node->next;
    }

    bool v_in_w = false;
    adj_list_t *node = graph->graph[w];
    while(node != NULL)
    {
        if(node->vertex == v){
            v_in_w = true;
            break;
        }
        node = node->next;
    }

    return v_in_w && w_in_v;
}

void graph_addedge(graph_t *graph, vertex_t v, vertex_t w)
//@requires G != NULL
//@requires v < graph_size(G) && w < graph_size(graph)
//@requires v != w && !graph_hasedge(G, v, w)
{
    // add node to v's adjacency list
    adj_list_t *node = (adj_list_t *) malloc(sizeof(adj_list_t));
    node->vertex = w;

    adj_list_t *search_node = graph->graph[v];
    graph->graph[v] = node;
    node->next = search_node;

    // add node to w's adjacency list
    node = (adj_list_t *) malloc(sizeof(adj_list_t));
    node->vertex = v;

    search_node = graph->graph[w];
    graph->graph[w] = node;
    node->next = search_node;
}

neighbor_t *graph_get_neighbors(graph_t *graph, vertex_t v)
//@requires graph != NULL && v < graph_size(graph)
//@ensures \result != NULL
{
    return (neighbor_t*)graph->graph[v];
}

bool graph_hasmore_neighbors(neighbor_t *nbor)
//@requires node != NULL
{
    adj_list_t *node = (adj_list_t*)nbor;
    return (node->next != NULL);
}

neighbor_t *graph_next_neighbor(neighbor_t *nbor)
//@requires node != NULL
//@requires graph_hasmore_neighbors(node)
{
    adj_list_t *node = (adj_list_t *)nbor;
    return (neighbor_t*)node->next;
}

void graph_print(graph_t *graph)
{
    neighbor_t *nbor;
    uint32_t graph_sz = graph_size(graph);
    for(int v=0; v < graph_sz; v++)
    {
        printf("[%d]",v);

        nbor = graph_get_neighbors(graph, v);
        if(nbor != NULL){
            printf(": ");
        }else{
            printf(" ");
        }

        while(graph_hasmore_neighbors(nbor)){
            printf("(%d) ", nbor->vertex);
            if(graph_hasmore_neighbors(nbor)) printf("-> ");
            nbor = graph_next_neighbor(nbor);
        }
        printf("\n");
    }
}