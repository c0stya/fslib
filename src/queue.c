#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "queue.h"

/*
FIFO queue implementation based on a cyclic unidirected likedlist.

*/

struct _queue * queue_create(size_t item_size) {
    struct _queue * q;

    q = (struct _queue *) malloc(sizeof(struct _queue));
    q->item_size = item_size;

    q->n_items = 0;

    q->head = NULL;
    q->tail = NULL;

    return q;
}

void queue_enque(struct _queue * q, void * item) {
    struct _qnode * node;

    assert ( q != NULL);

    node = (struct _qnode *)malloc(sizeof(struct _qnode) - 1 + q->item_size);
    node->next = NULL;

    memcpy(node->item, item, q->item_size);

    if (q->head == NULL)
        q->head = q->tail = node;
    else {
        q->tail->next = node;  
        q->tail = node;
    }

    ++q->n_items;
}

void * queue_deque(struct _queue * q, void * item) {
    struct _qnode * t_node;

    assert ( q != NULL );

    if (q->head == NULL)
        return NULL;    // Return NULL in case of empty queue
    
    t_node = q->head; 
    memcpy(item, t_node->item, q->item_size);

    q->head = t_node->next;
    free(t_node);

    --q->n_items;

    return item;
}    

void queue_empty(struct _queue * q) {
    void * item = malloc(q->item_size);

    while( queue_deque(q, item) != NULL );

    q->n_items = 0;
    free (item);
}

void queue_remove(struct _queue * q) {
    queue_empty(q);
    free (q);
}

