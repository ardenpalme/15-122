#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "queue.h"

/* O(N) time complexity */
bool queue_lookup(queue_t *queue, void *data_A, cmp_func_t cmp_func)
{
    ele_t *scan_ele = queue->head;
    while(scan_ele != queue->tail){
        if((*cmp_func)(data_A, scan_ele->data)) return true;
        scan_ele = scan_ele->next;
    }
    return false;
}

void enqueue(queue_t *queue, void *data)
//@requires queue != NULL && data != NULL
{
    ele_t *ele = (ele_t *)calloc(1,sizeof(ele_t));
    ele->data = data;

    if(queue->head == queue->tail && queue->head == NULL){
        //@assert(queue->size == 0)
        //@assert(queue->tail == NULL)
        queue->head = ele;
        queue->tail = ele;
    }else{
        queue->tail->next = ele;
        ele->prev = queue->tail;
        queue->tail = ele;
    }
    queue->size++;
}

void *dequeue(queue_t *queue)
//@requires queue != NULL 
//@requires queue->size > 0
{
    ele_t *ele = queue->head;
    if(queue->head == queue->tail) {
        //@assert(queue->size == 1)
        //@assert(queue->head != NULL)
        queue->head = NULL;
        queue->tail = NULL;
    }else{
        //@assert (queue->head->next != NULL)
        queue->head->next->prev = NULL;
        queue->head = queue->head->next;
    }
    queue->size--;

    void *ret = ele->data;
    free(ele);
    return ret;
}