#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "graph.h"

graph_t *graph_new(unsigned int num_vert)
//@ensures \result != NULL
{
    graph_t *graph = (graph_t *)calloc(1,sizeof(graph_t));
    graph->graph = (adj_list_t **)calloc(num_vert, sizeof(adj_list_t*));
    graph->size = num_vert;
    return graph;
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
    node = graph->graph[w];
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

void graph_addedge(graph_t *graph, vertex_t v, vertex_t w, uint32_t weight)
//@requires G != NULL
//@requires v < graph_size(G) && w < graph_size(graph)
//@requires v != w && !graph_hasedge(G, v, w)
{
    // add node to v's adjacency list
    adj_list_t *node = (adj_list_t *) malloc(sizeof(adj_list_t));
    node->vertex = w;
    node->weight = weight;

    adj_list_t *search_node = graph->graph[v];
    graph->graph[v] = node;
    node->next = search_node;

    // add node to w's adjacency list
    node = (adj_list_t *) malloc(sizeof(adj_list_t));
    node->vertex = v;
    node->weight = weight;

    search_node = graph->graph[w];
    graph->graph[w] = node;
    node->next = search_node;
}

void graph_remove_edge(graph_t *graph, vertex_t v, vertex_t w)
{
    node_t *search_node = graph->graph[v];
    node_t *node, *prev_node;

    node = graph->graph[v];
    prev_node = NULL;
    while(node != NULL) {
        if(node->vertex == w) {
            if(prev_node == NULL) {
                graph->graph[v] = node->next;
            }else{
                prev_node->next = node->next;
            }
            node->next = NULL;
            //free(node)
        }
        prev_node = node;
        node = node->next;
    }

    node = graph->graph[w];
    prev_node = NULL;
    while(node != NULL) {
        if(node->vertex == v) {
            if(prev_node == NULL) {
                graph->graph[w] = node->next;
            }else{
                prev_node->next = node->next;
            }
            node->next = NULL;
            //free(node)
        }
        prev_node = node;
        node = node->next;
    }
}

node_t *graph_get_neighbors(graph_t *graph, vertex_t v)
//@requires graph != NULL && v < graph_size(graph)
//@ensures \result != NULL
{
    return (node_t*)graph->graph[v];
}

bool graph_hasmore_neighbors(node_t *nbor)
//@requires node != NULL
{
    adj_list_t *node = (adj_list_t*)nbor;
    return (node->next != NULL);
}

node_t *graph_next_neighbor(node_t *nbor)
//@requires node != NULL
//@requires graph_hasmore_neighbors(node)
{
    adj_list_t *node = (adj_list_t *)nbor;
    return (node_t*)node->next;
}

void graph_print(graph_t *graph)
{
    node_t *node;
    uint32_t graph_sz = graph_size(graph);
    for(int i=0; i < graph_sz; i++)
    {
        printf("[%d]: ",i);

        node = graph->graph[i];
        while(node != NULL ){
            printf("(%d):[%d] ", node->vertex, node->weight);
            if(node->next != NULL) printf("-> ");
            node = node->next;
        }
        printf("\n");
    }
}

bool graph_has_cycle(graph_t *graph, bool *mark, vertex_t last_vert, vertex_t vert) 
{
    printf("vert: %d, last_vert: %d\n", vert, last_vert);
    mark[vert] = true;

    node_t *res;
    node_t *node = graph->graph[vert];
    while(node != NULL) {
        if(!mark[node->vertex]) {
            if(graph_has_cycle(graph, mark, vert, node->vertex)) return true;
        }else{
            if(node->vertex != last_vert) {
                printf("cycle detected! node: %d seen before!\n", node->vertex);
                return true;
            }
        }
        node = node->next;
    }

    if(node == NULL) return false; 
    return false;
}

bool graph_empty(graph_t *graph) {
    for(int i=0; i<graph->size; i++) {
        if(graph->graph[i] != NULL) return false;
    }
    return true;
}