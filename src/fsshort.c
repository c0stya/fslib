#include <stdlib.h>
#include "fst.h"

int main(int argc, char * argv[]) {
    struct _fst * fst = fst_create(); 
    struct _fst * path = fst_create(); 

    const char * ifilename = NULL; 
    const char * ofilename = NULL;

    FILE * fin = stdin;
    FILE * fout = stdout;

    if (argc > 1)
        ifilename = argv[1];
    if (argc > 2)
        ofilename = argv[2];
    
    if (ifilename) {
        if (( fin = fopen(ifilename, "rb")) == NULL ) {
            fprintf(stderr, "Error opening file: %s", ifilename);
            exit(EXIT_FAILURE);
        }
    }

    if (ofilename) {
        if (( fout = fopen(ofilename, "w")) == NULL ) {
            fprintf(stderr, "Error opening file: %s", ofilename);
            exit(EXIT_FAILURE);
        }
    }

    fst_read(fst, fin);
    fst_shortest(fst, path);
    fst_write(path, fout);

    fst_remove(fst);
    fst_remove(path);
    
    if (fin != stdout) {
        fclose(fin);
    }
    if (fout != stdout) {
        fclose(fout);
    }

    return 0;
}
