#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fst.h"
#include "queue.h"
#include "symt.h"

void fst_print(const struct _fst * fst) {
    struct _state state;
    struct _arc arc;

    state_t s;
    arc_t   a;
    
    struct _queue *finals = queue_create(sizeof(state_t));
    
    for(s=0; s < fst->n_states; ++s) {
        state = fst->states[s];
        for(a=0; a < state.n_arcs; ++a) {
            arc = state.arcs[a];

            printf("%"PRIu32"\t%"PRIu32"\t%"PRIu32"\t%"PRIu32"\t%.5f\n",
                s,
                arc.state,
                arc.ilabel,
                arc.olabel,
                arc.weight);
        }

        if(state.final == 1) {
            queue_enque(finals, &s);
        }
    }

    while(queue_deque(finals, &s) != NULL) {
        state = fst->states[s];
        printf("%"PRIu32"\t%f\n", s, state.weight);
    }

    queue_remove(finals);
}

typedef char * (*rtrans_f)(struct _symt * st, size_t id, char * token);

static char * trn(struct _symt * st, size_t id, char * token) {
    sprintf(token, "%zu", id);
    return token;
}

static char * trt(struct _symt * st, size_t id, char * token) {
    char * t = symt_get(st, id);
    if (t == NULL) {
        token = NULL;
        fprintf(stderr, "Unknown token: %s\n", token);
    } else {
        strcpy(token, t);
    }
    return token;
}


void fst_print_sym(
                    const struct _fst * fst,
                    struct _symt * ist,
                    struct _symt * ost,
                    struct _symt * sst) 
{
    struct _state state;
    struct _arc arc;

    token_t sa, sb, li, lo;

    rtrans_f strans, itrans, otrans; 

    state_t s;
    arc_t   a;

    itrans = (ist == NULL ? trn:trt);
    otrans = (ost == NULL ? trn:trt);
    strans = (sst == NULL ? trn:trt);

    struct _queue *finals = queue_create(sizeof(state_t));
    
    for(s=0; s < fst->n_states; ++s) {
        state = fst->states[s];
        for(a=0; a < state.n_arcs; ++a) {
            arc = state.arcs[a];

            strans(sst, s, sa);
            strans(sst, arc.state, sb); 
            itrans(ist, arc.ilabel, li);
            otrans(ost, arc.olabel, lo);

            if (sa == NULL || sb == NULL || li == NULL || lo == NULL) {
                fprintf(stderr, "Invalid symbol\n");
                exit(EXIT_FAILURE);
            }

            printf("%s\t%s\t%s\t%s\t%.5f\n",
                sa,
                sb,
                li,
                lo,
                arc.weight);
        }

        if(state.final == 1) {
            queue_enque(finals, &s);
        }
    }

    while(queue_deque(finals, &s) != NULL) {
        state = fst->states[s];

        strans(sst, s, sa);

        if (sa == NULL) {
            fprintf(stderr, "Invalid symbol\n");
            exit(EXIT_FAILURE);
        }

        printf("%s\t%f\n", sa, state.weight);
    }

    queue_remove(finals);
}

