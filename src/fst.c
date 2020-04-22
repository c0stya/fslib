#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "fst.h"

struct _fst * fst_create() {
    struct _fst * fst;
    fst = malloc(sizeof(struct _fst));

    fst->sr_type = SR_TROPICAL;
    fst->n_states = 0;
    fst->n_max = 0;
    fst->start = 0;

    fst->flags = 0;

    fst->states = NULL;

    return fst;
}

state_t fst_add_state(struct _fst * fst) {
    struct _state * state;
    ++fst->n_states;

    if (fst->n_states > fst->n_max) {
        fst->n_max = fst->n_states * 2;
        fst->states = realloc(fst->states, 
            sizeof(struct _state) * fst->n_max);
    }
    
    state = &fst->states[fst->n_states - 1];

    state->n_arcs = 0;
    state->n_max = 0;
    state->final = 0; 
    state->weight = 0;

    state->arcs = NULL;
    
    return fst->n_states - 1;
}

arc_t fst_add_arc(
            struct _fst *   fst, 
            state_t         src, 
            state_t         dst, 
            label_t         il,
            label_t         ol,
            weight_t        weight) 
{
    struct _arc * arc;
    struct _state * state = &fst->states[src];

    ++state->n_arcs;

    if (state->n_arcs > state->n_max) {
        state->n_max = state->n_arcs * 2;
        state->arcs = realloc(state->arcs, 
            sizeof(struct _arc) * state->n_max);
    }

    arc = &state->arcs[state->n_arcs - 1];
    arc->state = dst;
    arc->ilabel = il;
    arc->olabel = ol;
    arc->weight = weight;

    return state->n_arcs - 1;
}

void fst_remove(struct _fst * fst) {
    fst_empty(fst);
    free(fst);
}

void fst_empty(struct _fst * fst) {
    struct _state * state;

    for(state_t s=0; s < fst->n_states; ++s) {
        state = fst->states + s;
        if (state->arcs != NULL)
            free(state->arcs);
    } 
    
    if ( fst->states != NULL )
        free(fst->states);

    fst->n_states = 0;
    fst->n_max = 0;
    fst->states = NULL;
    fst->start = 0;
}

void fst_set_final(struct _fst * fst, state_t s, weight_t w) {
    (&fst->states[s])->final = 1;
    (&fst->states[s])->weight = w;
}

// maybe I should replace it with a compact representation
void fst_write(const struct _fst * fst, FILE * fout) {
    struct _state * state;

    fwrite( &_fst_header, sizeof(_fst_header), 1, fout);
    fwrite( &fst->start, sizeof(state_t), 1, fout);
    fwrite( &fst->n_states, sizeof(state_t), 1, fout);
    fwrite( &fst->sr_type, sizeof(uint8_t), 1, fout);
    fwrite( &fst->flags, sizeof(uint8_t), 1, fout);

    for (int s=0; s < fst->n_states; ++s) {
        state = &fst->states[s];  
        
        fwrite( &state->weight, sizeof(weight_t), 1, fout);
        fwrite( &state->n_arcs, sizeof(arc_t), 1, fout);
        fwrite( &state->final, sizeof(int), 1, fout);

        fwrite( state->arcs, sizeof(struct _arc), state->n_arcs, fout);
    }
}

void fst_read(struct _fst * fst, FILE * fin) {
    struct _state * state;
    uint32_t header = 0;

    fread( &header, sizeof(header), 1, fin);
    if (header != _fst_header) {
        printf("Wrong file format\n");
        exit(EXIT_FAILURE);
    }

    fread( &fst->start, sizeof(state_t), 1, fin);
    fread( &fst->n_states, sizeof(state_t), 1, fin);
    fread( &fst->sr_type, sizeof(uint8_t), 1, fin);
    fread( &fst->flags, sizeof(uint8_t), 1, fin);

    fst->n_max = fst->n_states;

    fst->states = malloc(sizeof(struct _state) * fst->n_states);

    for (int s=0; s < fst->n_states; ++s) {
        state = &fst->states[s];

        fread ( &state->weight, sizeof(weight_t), 1, fin);
        fread ( &state->n_arcs, sizeof(arc_t), 1, fin);
        fread ( &state->final, sizeof(int), 1, fin);

        state->n_max = state->n_arcs;

        state->arcs = malloc(sizeof(struct _arc) * state->n_arcs);
        fread ( state->arcs, sizeof(struct _arc), state->n_arcs, fin);
    }
}

void fst_fwrite(const struct _fst * fst, const char * filename) {
    FILE * fout = NULL;

    if (( fout = fopen(filename, "wb")) == NULL ) {
        fprintf(stderr, "Error opening file: %s", filename);
        exit(EXIT_FAILURE);
    }
    fst_write(fst, fout);
    fclose(fout); 
}

void fst_fread(struct _fst * fst, const char * filename) {
    FILE * fin = NULL;

    if (( fin = fopen(filename, "rb")) == NULL ) {
        fprintf(stderr, "Error opening file: %s", filename);
        exit(EXIT_FAILURE);
    }
    fst_read(fst, fin);
    fclose(fin); 
}

arc_t fst_get_n_arcs(const struct _fst * fst) {
    arc_t n = 0;
    for (state_t s = 0; s < fst->n_states; ++s) {
        n += fst->states[s].n_arcs; 
    }
    return n;
}

void fst_stack(struct _fst * a, const struct _fst * b) {
    struct _state * state;
    struct _arc * arc_tmp;

    state_t offset = a->n_states; 
    a->n_states = a->n_states + b->n_states; 

    /*
    Check if there is enough space for stacking.
    If not then allocate additional space.

    */

    if (a->n_max < a->n_states) {
        a->n_max = a->n_states; 
        a->states = realloc(a->states, a->n_max * sizeof(struct _state));
    }

    memcpy(&a->states[offset], b->states, b->n_states * sizeof(struct _state));

    for(state_t i=offset; i < a->n_states; ++i) {
        state = &a->states[i];
        arc_tmp = state->arcs;
        state->arcs = malloc(state->n_max * sizeof(struct _arc));
        memcpy(state->arcs, arc_tmp, state->n_arcs * sizeof(struct _arc));
        for (arc_t j=0; j < state->n_arcs; ++j)
            (&state->arcs[j])->state += offset;
    }
}

void fst_copy(const struct _fst * orig, struct _fst * copy) {
    struct _state * so, * sc;

    memcpy(copy, orig, sizeof(struct _fst));
    
    copy->states = malloc(sizeof(struct _state) * orig->n_max);
    memcpy(copy->states, orig->states, sizeof(struct _state) * orig->n_states);
    
    for(state_t s=0; s < copy->n_states; ++s) {
        sc = &copy->states[s];
        so = &orig->states[s];
        sc->arcs = malloc(sizeof(struct _arc) * sc->n_max);
        memcpy(sc->arcs, so->arcs, sizeof(struct _arc) * sc->n_arcs);
    }
}

void fst_relabel(struct _fst * fst, label_t old, label_t new, int dir) {
    // change labels through the whole automaton:
    // 0 - input 
    // 1 - output 
    
    struct _state * state;
    struct _arc * arc;

    for (state_t s = 0; s < fst->n_states; ++s) {
        state = &fst->states[s];
        for (arc_t a = 0; a < state->n_arcs; ++a) {
            arc = &state->arcs[a];

            if (dir == 0) {
                if (arc->ilabel == old) arc->ilabel = new; 
            } else {
                if (arc->olabel == old) arc->olabel = new; 
            }
        }
    }
}

