#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h>

/*
Simple queue structure with FIFO discipline.

*/

struct _queue {
    struct _qnode * head; 
    struct _qnode * tail;
    size_t item_size;
    size_t n_items;
};

struct _qnode {
    struct _qnode *     next;
    char                item[1];
};

struct _queue *     queue_create(size_t item_size);
void                queue_remove(struct _queue * q);
void                queue_empty(struct _queue * q);
void                queue_enque(struct _queue * q, void * item);
void *              queue_deque(struct _queue * q, void * item);

#endif
