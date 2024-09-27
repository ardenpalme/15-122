#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "graph_search.h"
#include "queue.h"

node_t *dfs_search(graph_t *graph, bool *mark, vertex_t start_vert, vertex_t target_vert)
//@requires graph != NULL
//@requires start_vert < graph_size(graph) && trarget_vert < graph_size(graph)
{
    mark[start_vert] = true;

    node_t *res;
    node_t *node = graph->graph[start_vert];
    while(node != NULL) {
        if(!mark[node->vertex]) {
            if(node->vertex == target_vert) break;

            res = dfs_search(graph, mark, node->vertex, target_vert);
            if(res != NULL) {
                printf("%d ", res->vertex);
                break;
            }
        }
        node = node->next;
    }

    if(node == NULL) return NULL; 
    return node;
}

bool cmp_func(void *data_A, void *data_B)
{
    node_t *n1 = (node_t *)data_A;
    node_t *n2 = (node_t *)data_B;

    return (n2->vertex == n1->vertex);
}

queue_t *bfs_search(graph_t *graph, bool *mark, vertex_t start_vert, vertex_t target_vert)
//@requires graph != NULL
//@requires start_vert < graph_size(graph) && trarget_vert < graph_size(graph)
{
    node_t *res, *node;
    queue_t *path = NULL;

    path = (queue_t *)malloc(sizeof(queue_t));
    node = graph->graph[start_vert];

    vertex_t vert = start_vert;

    while(1) {
        mark[vert] = true;
        node = graph->graph[vert];
        while(node != NULL) {
            if(node->vertex == target_vert) return path;

            if(!mark[node->vertex]) { 
                enqueue(path, (void*)node);
                vert = node->vertex;
                break;
            }
            node = node->next;
        }
        if(node == NULL) node = dequeue(path);
    }
}