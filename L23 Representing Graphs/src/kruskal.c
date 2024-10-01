#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "graph_search.h"

void print_queue_func(const void *data) 
//@requires queue != NULL 
{
    vertex_t *tmp = (vertex_t*)data;
    printf("%d",*tmp);
}

graph_t *spanning_tree(graph_t *graph)
//@requires is_graph(graph)
//@ensures is_tree(\result)
{
    graph_t *span_tree;
    node_t *node, *last_added_node ;
    bool *mark;
    vertex_t start_vert, vert;
    vertex_t *vertex;
    queue_t *work_list;

    span_tree = graph_new(graph->size);
    mark = (bool *)calloc(graph->size, sizeof(bool));
    work_list = (queue_t*)calloc(1, sizeof(queue_t));

    srand(time(0));
    vert = (vertex_t)(rand() % graph->size); 
    printf("random vertex: %d\n", vert);

    vertex = (vertex_t*)malloc(sizeof(vertex_t));
    *vertex = vert;
    enqueue(work_list, (void*)vertex);

    while(work_list->size > 0) {
        print_queue(work_list,print_queue_func);
        vertex = (vertex_t*)dequeue(work_list);

        node_t *node = graph->graph[*vertex];
        mark[*vertex] = true;

        while(node != NULL) {
            if(!mark[node->vertex]) {
                graph_addedge(span_tree, *vertex, node->vertex, node->weight);
                mark[node->vertex] = true;

                vertex = (vertex_t*)malloc(sizeof(vertex_t));
                *vertex = node->vertex;
                enqueue(work_list, (void*)vertex);
            }
            node = node->next;
        }
        printf("==============\n");
        graph_print(span_tree);
    }

    return span_tree;
}

typedef struct _graph_edges_t {
    vertex_t u;
    vertex_t v;
    uint32_t weight;
} graph_edges_t;

void sort_edges(graph_t *graph)
{
    node_t *node;
    uint32_t graph_sz = graph_size(graph);
    for(int i=0; i < graph_sz; i++)
    {
        node = graph->graph[i];
        while(node != NULL ){
            printf("(%d,%d): %d\n", i, node->vertex, node->weight);
            node = node->next;
        }
    }
}