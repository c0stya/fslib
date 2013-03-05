#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "queue.h"

void test_queue_create() {
    struct _queue * q = queue_create(sizeof(int));
    
    assert ( q != NULL );
    assert ( q->head == NULL );
    assert ( q->tail == NULL );
    assert ( q->n_items == 0 );

    queue_remove(q);
}

void test_queue_enque() {
    struct _queue * q = queue_create(sizeof(int));

    int i, size = 3;
    int items[3] = {0, 1, 2};
    int item;

    for (i=0; i<size; ++i) {
        assert( q->n_items == i );
        queue_enque(q, &items[i]);
    }
    
    queue_deque(q, &item);
    assert ( item == 0);
    assert ( q->n_items == 2);
    queue_deque(q, &item);
    assert ( item == 1);
    assert ( q->n_items == 1);
    queue_deque(q, &item);
    assert ( item == 2);
    assert ( q->n_items == 0);
    assert ( NULL == queue_deque(q, &item) );
    assert ( NULL == queue_deque(q, &item) );

    queue_remove(q);
}

int main() {
    test_queue_create();
    test_queue_enque();
}
