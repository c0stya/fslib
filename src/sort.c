#include <stdlib.h>
#include <stdio.h>

#include "fst.h"

static int icomp(const void *a, const void *b)
{
  return (int) ( ((struct _arc *)a)->ilabel - ((struct _arc *)b)->ilabel );
}

static int ocomp(const void *a, const void *b)
{
  return (int) ( ((struct _arc *)a)->olabel - ((struct _arc *)b)->olabel );
}

void fst_arc_sort(struct _fst * fst, int sort_outer) {
    struct _state * state;

    int (*comp)(const void *, const void *);

    if (sort_outer == 0)    {
        comp=icomp;
        fst->flags |= ISORT;
    } else {
        comp=ocomp;
        fst->flags |= OSORT;
    }
    
    for(state_t s = 0; s < fst->n_states; ++s) {
        state = &fst->states[s];
        qsort(  state->arcs, 
                state->n_arcs,
                sizeof(struct _arc),
                comp );
    }
}
