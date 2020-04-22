#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>
#include "fst.h"

static const char * header =
    "digraph T {\n"
    "\trankdir = LR;\n"
    "\torientation = Landscape;\n";
    
static const char * footer = "}\n";

int fst_draw(const struct _fst * fst, FILE * fout) {
    struct _state * state;
    struct _arc * arc;

    fprintf(fout, "%s", header);

    for(state_t s=0; s < fst->n_states; ++s) {
        state = &fst->states[s];

        if (!state->final) {
            fprintf(fout, "\t%"PRIu32" [label = \"%"PRIu32"\", shape = circle, style = %s ];\n", 
                s, s, s == fst->start ? "filled" : "solid");
        } else { 
            fprintf(fout, "\t%"PRIu32" [label = \"%"PRIu32"\", shape = doublecircle, style = filled ];\n", 
                s, s);
        }

        for(arc_t a=0; a < state->n_arcs; ++a) {
            arc = &state->arcs[a];
            fprintf(fout,
                "\t\t%"PRIu32" -> %"PRIu32" [ label = \"%"PRIu32":%"PRIu32"/%g\" ];\n",
                s, arc->state, 
                arc->ilabel, arc->olabel, 
                arc->weight);
        } 
    }

    fprintf(fout, "%s", footer);

    return 0;
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

int fst_draw_sym(   const struct _fst * fst, 
                    FILE * fout,
                    struct _symt * ist,
                    struct _symt * ost,
                    struct _symt * sst)
{
    struct _state * state;
    struct _arc * arc;

    token_t sa, sb, li, lo;
    rtrans_f strans, itrans, otrans; 

    itrans = (ist == NULL ? trn:trt);
    otrans = (ost == NULL ? trn:trt);
    strans = (sst == NULL ? trn:trt);


    fprintf(fout, "%s", header);

    for(state_t s=0; s < fst->n_states; ++s) {
        state = &fst->states[s];

        strans(sst, s, sa);
        
        if (sa == NULL) {
            fprintf(stderr, "Invalid symbol\n");
            exit(EXIT_FAILURE);
        }

        if (!state->final) {
            fprintf(fout, "\t%s [label = \"%s\", shape = circle, style = %s ];\n", 
                sa, sa, s == fst->start ? "filled" : "solid");
        } else { 
            fprintf(fout, "\t%s [label = \"%s\", shape = doublecircle, style = filled ];\n", 
                sa, sa);
        }

        for(arc_t a=0; a < state->n_arcs; ++a) {
            arc = &state->arcs[a];

            strans(sst, arc->state, sb); 
            itrans(ist, arc->ilabel, li);
            otrans(ost, arc->olabel, lo);

            if (sb == NULL || li == NULL || lo == NULL) {
                fprintf(stderr, "Invalid symbol\n");
                exit(EXIT_FAILURE);
            }

            fprintf(fout,
                "\t\t%s -> %s [ label = \"%s:%s/%g\" ];\n",
                sa, sb, li, lo, arc->weight);
        } 
    }

    fprintf(fout, "%s", footer);

    return 0;
}
