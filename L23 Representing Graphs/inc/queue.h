#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct _ele_t {
    void *data;
    struct _ele_t *next;
    struct _ele_t *prev;
} ele_t;

typedef bool (*cmp_func_t) (const void *ele1, const void *ele2);

typedef struct _queue_t {
    ele_t *head;
    ele_t *tail;
    uint32_t size;
} queue_t;

void enqueue(queue_t *queue, void *data);
//@requires queue != NULL && data != NULL

/* O(N) time complexity */
bool queue_lookup(queue_t *queue, void *data, cmp_func_t cmp_func);

void *dequeue(queue_t *queue);
//@requires queue != NULL 

#endif /* __QUEUE_H__ */