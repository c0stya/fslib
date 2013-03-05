#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "sr.h"
#include "fst.h"

static void _add_arc(
        struct _fst * fst,
        size_t sa,
        size_t sb,
        size_t li,
        size_t lo,
        float w
) {
    while((sa + 1 > fst->n_states) || (sb + 1> fst->n_states))
        fst_add_state(fst);

    fst_add_arc(fst, sa, sb, (label_t)li, (label_t)lo, w);
}

static void _add_final(
        struct _fst * fst,
        size_t s,
        float w
) {
    while(s + 1 > fst->n_states)
        fst_add_state(fst);

    fst_set_final(fst, s, w);
}

static int _parse_line(struct _fst * fst, char * buf) {
    size_t sa, sb, sf, li, lo;
    weight_t w;
    struct _sr sr = sr_get(fst->sr_type);

    if (sscanf(buf, "%zd\t%zd\t%zd\t%zd\t%f",   // TODO: add marcro instead of %f
           &sa, &sb, &li, &lo, &w) == 5) {
        _add_arc(fst, sa, sb, li, lo, w);
    } else {
        if (sscanf(buf, "%zd\t%zd\t%zd\t%zd",   // TODO: add marcro instead of %f
               &sa, &sb, &li, &lo) == 4) {
            _add_arc(fst, sa, sb, li, lo, sr.one);
        } else {
            if (sscanf(buf, "%zd\t%f", &sf, &w) == 2) { // TODO: add macro instead of %f
                _add_final(fst, sf, w);
            } else {
                if (sscanf(buf, "%zd", &sf) == 1)
                    _add_final(fst, sf, sr.one);          // TODO: change to semiring 1!
                else
                    return -1;
            }
        }
    }
    return 0;
}

struct _fst * fst_compile(struct _fst * fst, FILE * fin) {
    char p[0xffff];
    size_t line = 1;

    while (fgets(p, sizeof(p), fin) != NULL) {
        ++line;
        if ( _parse_line(fst, p) != 0) {
                printf("Invalid input line %zd: %s", line, p);
                exit(EXIT_FAILURE);
        }
    }

    return fst;
}



struct _fst * fst_compile_str(struct _fst * fst, const char * str) {
    char p[0xffff];
    char * tok;
    size_t line = 1;

    strcpy(p, str);

    do {
        if (line == 1)
            tok = strtok(p, "\n");
        else {
            tok = strtok(NULL, "\n");
        }

        if (tok)
            if ( _parse_line(fst, tok) != 0) {
                    printf("Invalid input line %zd: %s", line, tok);
                    exit(EXIT_FAILURE);
            }
        ++line;
    } while (tok);

    return fst;
}


