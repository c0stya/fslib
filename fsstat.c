#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "fst.h"

int main(int argc, char * argv[]) {
    struct _fst * fst = fst_create(); 

    const char * ifilename = NULL; 
    FILE * fin = stdin;

    if (argc > 1)
        ifilename = argv[1];

    if (ifilename) {
        if (( fin = fopen(ifilename, "rb")) == NULL ) {
            fprintf(stderr, "Error opening file: %s", ifilename);
            exit(EXIT_FAILURE);
        }
    }

    fst_read(fst, fin);

    printf("states:\t\t%"PRIu32"\n", fst->n_states);
    printf("arcs:\t\t%"PRIu32"\n", fst_get_n_arcs(fst));
    printf("sorted input:\t%d\n", fst->flags & ISORT ? 1 : 0);
    printf("sorted output:\t%d\n", fst->flags & OSORT ? 1 : 0);
    printf("start state:\t%"PRIu32"\n", fst->start);

    fst_remove(fst);
    
    if (fin != stdout) {
        fclose(fin);
    }

    return 0;
}
