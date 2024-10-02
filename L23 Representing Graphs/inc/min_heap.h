#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#include <stdbool.h>

typedef uint64_t priority_t;
typedef void *heap_elem_t;
typedef priority_t (*priority_func_t)(heap_elem_t elem);
typedef void (*print_elem_func_t)(heap_elem_t elem);

typedef struct _heap_t {
    heap_elem_t data;
    uint32_t level;
    struct _heap_t  *parent;
    struct _heap_t  *left;
    struct _heap_t *right;
} heap_t;

typedef struct _min_heap_t {
    uint64_t num_elem;
    uint32_t max_level;
    heap_t *root;
    heap_t *last_node;
} min_heap_t;

void min_heap_add(min_heap_t *min_heap, 
                  heap_elem_t elem, 
                  priority_func_t priority_func);
//@requires min_heap != NULL 

heap_elem_t min_heap_rem(min_heap_t *min_heap, 
                         priority_func_t priority_func);
//@requires heap != NULL && !heap_empty(heap)

void min_heap_print(min_heap_t *min_heap, 
                    print_elem_func_t print_func);
//@requires min_heap != NULL 

#endif /* __MIN_HEAP_H__ */