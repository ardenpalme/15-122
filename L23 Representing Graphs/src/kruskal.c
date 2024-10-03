#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "kruskal.h"
#define INV_VERTEX 10000

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
                //printf("adding (%d,%d,%d)\n", i, node->vertex, node->weight);
                
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

graph_t *min_span_tree(graph_t *graph)
{

    graph_t *span_tree;
    heap_elem_t elem;
    graph_edge_t *edge;

    min_heap_t *min_heap = (min_heap_t*)calloc(1,sizeof(min_heap_t));
    bool **mark2d = malloc(graph->size * sizeof(bool *));
    for (int i = 0; i < graph->size; ++i) {
        mark2d[i] = malloc(graph->size * sizeof(bool));
    }

    bool *mark = calloc(graph->size, sizeof(bool));
    span_tree = graph_new(graph->size);

    sort_edges(graph, mark2d, min_heap);
    vertex_t span_tree_root = INV_VERTEX;

    while(min_heap->root) {
        elem = min_heap_rem(min_heap, priority_func);
        edge = (graph_edge_t*)elem;
        printf("considering (%d, %d, %d)\n", edge->u, edge->v, edge->weight);

        graph_addedge(span_tree, edge->u, edge->v, edge->weight);

        if(span_tree_root == INV_VERTEX) {
            span_tree_root = edge->u;
        }else{
            //@assert(span_tree_root != INV_VERTEX)
            memset(mark, 0, graph->size);
            printf("checking cycle: %d\n",span_tree_root);
            if(graph_has_cycle(span_tree, mark, INV_VERTEX, span_tree_root)){
                graph_remove_edge(span_tree, edge->u, edge->v);
            }
        }
        //min_heap_print(min_heap, edge_print_func);
        //printf("\n");
    }

    return span_tree;
}