#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "symt.h"
#include "hash.h"

static size_t fnv32(void * str) {

    const char * s = str;
    uint32_t hsh = FNV_OFFSET_32, i;

    for(i = 0; i < strlen(s); i++) {
        // xor next byte into the bottom of the hash
        hsh = hsh ^ (s[i]); 
        // Multiply by prime number found to work well
        hsh = hsh * FNV_PRIME_32; 
    }
    return hsh;
}

static int str_cmp(void * a, void * b) {
    return strcmp((const char *)a, (const char *)b) == 0 ? 1: 0;
}

struct _symt * symt_read(struct _symt * st, FILE * fin) {
    char p[0xffff];
    char token[TOKEN_SIZE];
    size_t line = 1;
    size_t token_id;

    while (fgets(p, sizeof(p), fin) != NULL) {
        if (sscanf(p, "%s\t%zu", token, &token_id) == 2) {
            symt_add(st, token_id, token);
        }
        else {
                printf("Invalid input line %zd: %s", line, p);
                exit(EXIT_FAILURE);
        }
        ++line;
    }
    return st;
}

struct _symt * symt_compile(struct _symt * st, const char * str) {
    char p[0xffff];
    char token[TOKEN_SIZE];
    char * tok;
    size_t line = 1;
    size_t token_id;

    strcpy(p, str);

    do {
        if (line == 1)
            tok = strtok(p, "\n");
        else {
            tok = strtok(NULL, "\n");
        }

        if (tok) {
            if (sscanf(p, "%s\t%zd", token, &token_id) == 2) {
                symt_add(st, token_id, token);
            }
            else {
                    printf("Invalid input line %zd: %s", line, p);
                    exit(EXIT_FAILURE);
            }
        }
        ++line;
    } while (tok);

    return st;
}    

struct _symt * symt_fread(struct _symt * st, const char * filename) {
    FILE * fin = NULL;
    if (( fin = fopen(filename, "r")) == NULL ) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    st = symt_read(st, fin);
    if (fin != NULL)
        fclose(fin);
    return st;
}   

struct _symt * symt_create() {
    struct _symt * st = malloc(sizeof(struct _symt));

    st->rev = NULL;
    st->sym = malloc(SYM_INIT_SIZE * sizeof(char *));

    st->n_items = 0;
    st->n_max = SYM_INIT_SIZE;

    for(size_t i=0; i < st->n_max; ++i)
        st->sym[i] = NULL;

    return st;
}

void symt_remove(struct _symt * st) {
    for(size_t i=0; i < st->n_max; ++i) 
        if(st->sym[i] != NULL)  
            free(st->sym[i]);

    if (st->rev != NULL)
        hash_remove(st->rev);

    free(st->sym);
    free(st);
}

char * symt_add(
                        struct _symt * st, 
                        size_t id, 
                        const char * token) {

    // Token should be a C-string
    size_t s = st->n_max;
    char ** p;

    assert(st->sym != NULL);
    assert(st->n_max != 0);

    while (st->n_max <= id) {
        st->n_max *= 2;
    }

    if( st->n_max > s) {
        p = realloc(st->sym, st->n_max * sizeof(char *)); 
        st->sym = p;
    }

    for(size_t i = s; i < st->n_max; ++i)
        st->sym[i] = NULL;

    char * t = malloc(strlen(token) + 1);
    //strncpy(t, token, TOKEN_SIZE);
    strcpy(t, token);
    st->sym[id] = t;
    ++st->n_items;

    return st->sym[id];
}

void symt_print(struct _symt * st) {
    for(size_t i=0; i < st->n_max; ++i) {
        if ( st->sym[i] != NULL )
            printf("%s\t%zd\n", st->sym[i], i);
    } 
} 

struct _hash * symt_reverse(struct _symt * st) {
    if(st->rev != NULL)
        hash_remove(st->rev);

    st->rev = hash_create(
                    fnv32,
                    str_cmp,
                    TOKEN_SIZE,
                    sizeof(size_t),
                    st->n_items * 2);

    char token[TOKEN_SIZE] = {0};

    for(size_t i=0; i < st->n_max; ++i) {
        if ( st->sym[i] != NULL ) {
            strncpy(token, st->sym[i], TOKEN_SIZE - 1);
            //strcpy(token, st->sym[i]);
            hash_add(st->rev, token, &i);
            //hash_add(st->rev, st->sym[i], &i);
        }
    } 

    return st->rev;
}

size_t symt_getr(struct _symt * st, char * token) {
    size_t id = -1;
    if (st->rev == NULL)
       symt_reverse(st); 

    hash_get(st->rev, token, &id); 

    return id; 
}

char * symt_get(struct _symt * st, size_t id) {
    if ( id > st->n_items ) 
        return NULL;
    return st->sym[id];
}
