#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
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
            fprintf(fout, "\t%"PRIu32" [label = \"%"PRIu32"/%g\", shape = doublecircle, style = filled ];\n", 
                s, s, state->weight);
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
