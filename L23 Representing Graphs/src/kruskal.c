#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "kruskal.h"

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

static priority_t priority_func(heap_elem_t elem)
{
    graph_edge_t *edge = (graph_edge_t*)elem;
    return edge->weight;
}

static void print_func(heap_elem_t elem) {
    graph_edge_t *edge = (graph_edge_t  *)elem;
    printf("%d", edge->weight);
}

void sort_edges(graph_t *graph, bool **mark, min_heap_t *min_heap)
{
    node_t *node;
    uint32_t graph_sz = graph_size(graph);
    graph_edge_t *edge;

    for(int i=0; i < graph_sz; i++)
    {
        node = graph->graph[i];
        while(node != NULL ){
            if(!mark[i][node->vertex])
            {
                printf("adding (%d,%d,%d)\n", i, node->vertex, node->weight);
                
                edge = (graph_edge_t*)malloc(sizeof(graph_edge_t));
                edge->u = i;
                edge->v = node->vertex;
                edge->weight = node->weight;

                min_heap_add(min_heap, (void*)edge, priority_func);
                //min_heap_print(min_heap, print_func);

                mark[i][node->vertex] = true;
                mark[node->vertex][i] = true;
            }
            node = node->next;
        }
    }
}
