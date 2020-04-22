#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "sr.h"
#include "fst.h"
#include "symt.h"


// TODO: add const modifier
typedef size_t (*trans_f)(char * token, struct _symt * st);

static size_t trn(char * token, struct _symt * st) {
    char * end;
    long dig = strtol(token, &end, 10);
    if (!*end)
        return (size_t)dig;
    
    fprintf(stderr, "Incorrect token: %s\n", token);

    return -1;
}

// TODO: add const modifier
static size_t trt(char * token, struct _symt * st) {
    size_t t = symt_getr(st, token);
    if (-1 == t) {
        fprintf(stderr, "Unknown token: %s\n", token);
    }
    return t;
}

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

static int _parse_line_sym(
                                struct _fst * fst, 
                                char * buf,
                                struct _symt * ist,
                                struct _symt * ost,
                                struct _symt * sst ) 
{
    trans_f strans, itrans, otrans;

    token_t sa, sb, sf, li, lo;
    size_t _sa, _sb, _sf, _li, _lo;

    weight_t w;
    struct _sr sr = sr_get(fst->sr_type);

    itrans = (ist == NULL ? trn:trt);
    otrans = (ost == NULL ? trn:trt);
    strans = (sst == NULL ? trn:trt);

    if (sscanf(buf, "%s\t%s\t%s\t%s\t%f",   // TODO: add marcro instead of %f
           sa, sb, li, lo, &w) == 5) {

        _sa = strans(sa, sst);
        _sb = strans(sb, sst);
        _li = itrans(li, ist); 
        _lo = otrans(lo, ost); 

        if ( _sa == -1 || _sb == -1 || _li == -1 || _lo == -1 ) 
            return -1;

        _add_arc(fst, _sa, _sb, _li, _lo, w);
    } else {
        if (sscanf(buf, "%s\t%s\t%s\t%s",   // TODO: add marcro instead of %f
               sa, sb, li, lo) == 4) {

            _sa = strans(sa, sst);
            _sb = strans(sb, sst);
            _li = itrans(li, ist); 
            _lo = otrans(lo, ost); 

            if ( _sa == -1 || _sb == -1 || _li == -1 || _lo == -1 ) 
                return -1;

            _add_arc(fst, _sa, _sb, _li, _lo, sr.one);
        } else {
            if (sscanf(buf, "%s\t%f", sf, &w) == 2) { // TODO: add macro instead of %f
                _sf = strans(sf, sst);
                
                if( -1 == _sf)
                    return -1;

                _add_final(fst, _sf, w);
            } else {
                if (sscanf(buf, "%s", sf) == 1) {
                    _sf = strans(sf, sst);

                    if( -1 == _sf )
                        return -1;

                    _add_final(fst, _sf, sr.one);          // TODO: change to semiring 1!
                } else
                    return -1;
            }
        }
    }
    return 0;
}

static int _parse_line_sym_acc(
                                struct _fst * fst, 
                                char * buf,
                                struct _symt * ist,
                                struct _symt * sst ) 
{
    trans_f strans, itrans;

    token_t sa, sb, sf, li;
    size_t _sa, _sb, _sf, _li;

    weight_t w = -1.0;
    struct _sr sr = sr_get(fst->sr_type);

    itrans = (ist == NULL ? trn:trt);
    strans = (sst == NULL ? trn:trt);

    if (sscanf(buf, "%s\t%s\t%s\t%f",   // TODO: add marcro instead of %f
           sa, sb, li, &w) == 4) {

        _sa = strans(sa, sst);
        _sb = strans(sb, sst);
        _li = itrans(li, ist); 

        if ( _sa == -1 || _sb == -1 || _li == -1 ) 
            return -1;

        _add_arc(fst, _sa, _sb, _li, _li, w);
    } else {
        if (sscanf(buf, "%s\t%s\t%s",   //
               sa, sb, li) == 3) {


            _sa = strans(sa, sst);
            _sb = strans(sb, sst);
            _li = itrans(li, ist); 

            if ( _sa == -1 || _sb == -1 || _li == -1 ) 
                return -1;

            _add_arc(fst, _sa, _sb, _li, _li, sr.one);
        } else {
            if (sscanf(buf, "%s\t%f", sf, &w) == 2) { // TODO: add macro instead of %f
                _sf = strans(sf, sst);
                
                if( -1 == _sf)
                    return -1;

                _add_final(fst, _sf, w);
            } else {
                if (sscanf(buf, "%s", sf) == 1) {
                    _sf = strans(sf, sst);

                    if( -1 == _sf )
                        return -1;

                    _add_final(fst, _sf, sr.one);          // TODO: change to semiring 1!
                } else
                    return -1;
            }
        }
    }
    return 0;
}


struct _fst * fst_compile(
                            struct _fst * fst, 
                            FILE * fin,
                            struct _symt * ist,
                            struct _symt * ost,
                            struct _symt * sst,
                            int is_acc) 
{
    char p[0xffff];
    size_t line = 1;
    int res = 0;

    while (fgets(p, sizeof(p), fin) != NULL) {
        ++line;

        if (is_acc == 0)
            res = _parse_line_sym(fst, p, ist, ost, sst);
        else
            res = _parse_line_sym_acc(fst, p, ist, sst);
        
        if (res != 0) {
                fprintf(stderr, "Invalid input line %zd: %s", line, p);
                exit(EXIT_FAILURE);
        }
    }

    size_t start_state;

    if (sst != NULL) {
        if ( (start_state = symt_getr(sst, START_STATE)) != -1) 
            fst->start = start_state;
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
                    fprintf(stderr, "Invalid input line %zd: %s\n", line, tok);
                    exit(EXIT_FAILURE);
            }
        ++line;
    } while (tok);

    return fst;
}


