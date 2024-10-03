#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "min_heap.h"
#include "queue.h"

void print_func10(heap_elem_t elem) {
    printf("%d", *(uint32_t*)elem);
}

/* ==== CONDITIONALS - BEGIN ==== */

bool is_heap(heap_t *heap)
{
    if(heap == NULL) return true;

    return (heap->parent == NULL ||
            (heap->parent->right == heap || 
             heap->parent->left == heap)) &&
           is_heap(heap->right) && 
           is_heap(heap->left);
}

bool is_min_heap(heap_t *heap)
{
    (void)heap;
    return true;
}

/* ==== CONDITIONALS - END ==== */

/* ==== HELPERS - BEGIN ==== */

bool print_heap(heap_t *heap, print_elem_func_t print_func)
{
    if(heap == NULL) return true;

    (*print_func)(heap->data); //printf("%d:[", *(int*)heap->data);
    printf(": [");

    if(heap->left){
        (*print_func)(heap->left->data); //printf("%d, ", *(int*)heap->left->data);
        printf(", ");
    } else printf(",");

    if(heap->right){
        (*print_func)(heap->right->data); //printf("%d, ", *(int*)heap->right->data);
        printf(", ");
    } else printf(",");

    if(heap->parent){
        (*print_func)(heap->parent->data); //printf("%d", *(int*)heap->parent->data);
        printf(" ");
    } else printf(" ");
    printf("] @ %d\n", heap->level);

    return print_heap(heap->left, print_func) && 
           print_heap(heap->right, print_func);
}

heap_t *swap_up(heap_t *heap)
//@requires heap != NULL && heap->parent != NULL
{
    heap_t *parent, *grandparent;

    parent = heap->parent;    
    if(parent->right == heap) {
        parent->right = heap->right;
        heap->right = parent;

        heap_t *parent_left_tree = parent->left;
        parent->left = heap->left;
        heap->left = parent_left_tree;
        if(parent_left_tree) parent_left_tree->parent = heap;

    } else if(parent->left == heap) {
        parent->left = heap->left;
        heap->left = parent;

        heap_t *parent_right_tree = parent->right;
        parent->right = heap->right;
        heap->right = parent_right_tree;
        if(parent_right_tree) parent_right_tree->parent = heap;
    }

    grandparent = heap->parent->parent;
    if(grandparent != NULL) {
        if(grandparent->right == parent) {
            grandparent->right = heap;

        } else if(grandparent->left == parent) {
            grandparent->left = heap;
        }
    }

    if(parent->right) parent->right->parent = parent;
    if(parent->left) parent->left->parent = parent;

    heap->parent = grandparent;
    if(heap->left) heap->left->parent = heap;
    if(heap->right) heap->right->parent = heap;

    parent->level++;
    if(heap->level != 0) heap->level--;

    return heap;
}

heap_t *get_next_heap_node(heap_t *heap, uint32_t max_level) 
//@requires min_heap != NULL 
{
    heap_t *left_node, *right_node;
    if(heap == NULL) return NULL;

    //printf("node: [%d]\n", *(int*)heap->data);

    if(heap->level == max_level) {
       if(heap->left == NULL || heap->right == NULL) return heap;
       else return NULL;

    } else if(heap->level < max_level) {
        left_node = get_next_heap_node(heap->left, max_level);
        if(left_node) return left_node;

        //@assert(!left_node)
        right_node = get_next_heap_node(heap->right, max_level);
        if(right_node) return right_node;
        else           return NULL;

    }else{
        //@assert(heap->level > max_level)
        return NULL;
    }
}

heap_t *get_next_rem_node(heap_t *heap, uint32_t max_level) 
{
    heap_t *left_node, *right_node;
    if(heap == NULL) return NULL;

    //printf(">> max(%d) node: [%d]\n", max_level, *(int*)heap->data);

    if(heap->level == max_level) {
       //@assert(heap->left == NULL && heap->right == NULL) 
       return heap;

    } else if(heap->level < max_level) {
        right_node = get_next_rem_node(heap->right, max_level);
        if(right_node) return right_node;

        //@assert(!right_node)
        left_node = get_next_rem_node(heap->left, max_level);
        if(left_node) return left_node;
        else           return NULL;

    }else{
        //@assert(heap->level > max_level)
        return NULL;
    }
}

heap_t *get_next_heap(min_heap_t *min_heap) 
//@requires min_heap != NULL 
{
    uint32_t max_level = (min_heap->max_level > 0) ? (min_heap->max_level - 1) : 0;

    heap_t *heap = get_next_heap_node(min_heap->root, max_level);

    if(heap == NULL) printf("couldn't find next heap node\n");

    return heap;
}

heap_t *get_next_swap_node(heap_t *heap, priority_func_t priority_func)
{
    if(heap->left != NULL && heap->right != NULL) {
        priority_t prior = (*priority_func)(heap->data);
        priority_t prior_right = (*priority_func)(heap->right->data);
        priority_t prior_left = (*priority_func)(heap->left->data);

        if(prior > prior_right && prior > prior_left) {
            if(prior_left > prior_right){
                return heap->right;
            }else{
                return heap->left;
            }
        }
        if(prior < prior_right && prior < prior_left) return NULL;
    }

    if((heap->right != NULL) &&
       ((*priority_func)(heap->data) > (*priority_func)(heap->right->data))) {
        return heap->right;
    }

    if((heap->left != NULL) &&
       ((*priority_func)(heap->data) > (*priority_func)(heap->left->data))) {
        return heap->left;
    }

    return NULL;
}

/* ==== HELPERS - END ==== */

void min_heap_add(min_heap_t *min_heap, 
                  heap_elem_t elem, 
                  priority_func_t priority_func)
//@requires min_heap != NULL 
{
    heap_t *node = (heap_t*)malloc(sizeof(heap_t));
    node->data = elem;
    node->left = NULL;
    node->right = NULL;

    if(min_heap->root == NULL) {
        node->level = 0;
        min_heap->root = node;
        min_heap->last_node = node;
        min_heap->max_level = 0;
        min_heap->num_elem = 1;
        return;
    }

    if(min_heap->num_elem >= (int)pow(2,(min_heap->max_level))) {
        //printf("LVL %d\n", (int)pow(2,(min_heap->max_level)));
        min_heap->max_level++;
        min_heap->num_elem=0;
    }

    heap_t *parent_node = get_next_heap(min_heap);
    //printf("parent_node: %d\n", *(int*)parent_node->data);

    if(parent_node->left == NULL) {
        parent_node->left = node;

    }else if(parent_node->right == NULL) {
        parent_node->right = node;
    }else{
        perror("parent_node has 2 children");
        return;
    }

    node->level = parent_node->level + 1;
    node->parent = parent_node;

    bool last_node_updated = false;
    min_heap->last_node = node;

    // Restore ordering invariant
    while((node->parent != NULL) && 
          ((*priority_func)(node->data) < (*priority_func)(node->parent->data))) 
    {
        //printf("===== BEGIN =======\n");
        //print_heap(min_heap->root, print_func1);
        //printf("swap_up(%d)\n", *(int*)node->data);

        heap_t *parent = node->parent; 
        node = swap_up(node);

        if(!last_node_updated ){
            min_heap->last_node = parent;
            last_node_updated  = true;
        }

        heap_t *tmp = node;
        while(tmp->parent != NULL) {
            tmp = tmp->parent;
        }
        min_heap->root = tmp;

    if(!is_heap(min_heap->root)) fprintf(stderr, "min_heap invariant violated\n");

        //print_heap(min_heap->root);
        //printf("====== END ========\n");
    }

    if(!is_heap(min_heap->root)) fprintf(stderr, "min_heap invariant violated\n");

    min_heap->num_elem++;
}

heap_elem_t min_heap_rem(min_heap_t *min_heap, 
                         priority_func_t priority_func)
//@requires heap != NULL && !heap_empty(heap)
{
    if(min_heap->root->right == NULL && 
       min_heap->root->left == NULL) {
        heap_elem_t ret= min_heap->root->data;
        min_heap->root = NULL;
        return ret;
    }

    heap_t *node = get_next_rem_node(min_heap->root, min_heap->max_level);
    //printf("Removing node [%u]\n", *(uint32_t*)node->data);

    if(node != NULL) {
        if(node->parent->left == node) {
            node->parent->left = NULL;

        } else if(node->parent->right == node) {
            node->parent->right = NULL;
        }
    }

    heap_elem_t ret = min_heap->root->data;
    node->left = min_heap->root->left;
    if(node->left) node->left->parent = node;

    node->right = min_heap->root->right;
    if(node->right) node->right->parent = node;

    node->level = 0;
    node->parent = NULL;

    min_heap->root = node;


    // Restore ordering invariant
    heap_t *swap_node;
    while((swap_node=get_next_swap_node(node, priority_func)) != NULL)
    {
        //printf("===== BEGIN (REM) =======\n");
        //print_heap(min_heap->root);
        //printf("swap_up(%d)\n", *(uint32_t*)swap_node->data);

        //print_heap(min_heap->root, print_func10);
        //printf("node: %d\n", *(uint32_t*)node->data);
        //printf("swap_node: %d\n", *(uint32_t*)swap_node->data);

        (void)swap_up(swap_node);

        heap_t *tmp = node;
        while(tmp->parent != NULL) {
            tmp = tmp->parent;
        }
        min_heap->root = tmp;

        if(!is_heap(min_heap->root)) fprintf(stderr, "min_heap invariant violated\n");

        //print_heap(min_heap->root);
        //printf("====== END (REM) ========\n");

    }

    min_heap->num_elem--;
    if(min_heap->num_elem == 0) {
        min_heap->max_level--;
        min_heap->num_elem=(int)pow(2,(min_heap->max_level));
    }

    if(!is_heap(min_heap->root)) fprintf(stderr, "min_heap invariant violated\n");

    return ret;
}

void min_heap_print(min_heap_t *min_heap, 
                    print_elem_func_t  print_func)
//@requires min_heap != NULL 
{
    printf("==== Min Heap ====\n");
    printf("max level: %.2d\n", min_heap->max_level);
    printf("num elem: %.2llu\n", min_heap->num_elem);

    print_heap(min_heap->root, print_func);
}