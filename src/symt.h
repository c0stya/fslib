#ifndef __SYMT_H__
#define __SYMT_H__

#include <inttypes.h>
#include "hash.h"

#define SYM_INIT_SIZE 1024 //256
#define TOKEN_SIZE  1024 //256

#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U

typedef char token_t[TOKEN_SIZE];

struct _symt {
    size_t              n_items;
    size_t              n_max;
    char **             sym;
    struct _hash *      rev;
};

struct _symt *      symt_create();
void                symt_remove(struct _symt * st);
struct _hash *      symt_reverse(struct _symt * st);
struct _symt *      symt_read(struct _symt * st, FILE * fin);
struct _symt *      symt_fread(struct _symt * st, const char * filename);
//struct _symt *      symt_compile(struct _symt * st, const char * syms);
void                symt_print(struct _symt * st);
char *              symt_add(struct _symt * st, size_t id, const char * token);
size_t              symt_getr(struct _symt * st, char * token);
char *              symt_get(struct _symt * st, size_t id);

#endif
