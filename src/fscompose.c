#include <stdlib.h>
#include <string.h>
#include "fst.h"

int main(int argc, char * argv[]) {
    struct _fst * fst_a = fst_create(); 
    struct _fst * fst_b = fst_create(); 
    struct _fst * fst_c = fst_create(); 
    const char * ifilename_a = NULL; 
    const char * ifilename_b = NULL; 
    const char * ofilename = NULL;
    FILE * fin_a = stdin;
    FILE * fin_b = stdin;
    FILE * fout = stdout;

    if (argc > 1)
        ifilename_a = argv[1];
    if (argc > 2)
        ifilename_b = argv[2];
    if (argc > 3)
        ofilename = argv[3];
    
    if (ifilename_a && strcmp(ifilename_a, "-") != 0) {
        if (( fin_a = fopen(ifilename_a, "rb")) == NULL ) {
            fprintf(stderr, "Error opening file: %s", ifilename_a);
            exit(EXIT_FAILURE);
        }
    }

    if (ifilename_b && strcmp(ifilename_b, "-") != 0) {
        if (( fin_b = fopen(ifilename_b, "rb")) == NULL ) {
            fprintf(stderr, "Error opening file: %s", ifilename_b);
            exit(EXIT_FAILURE);
        }
    }

    if (ofilename) {
        if (( fout = fopen(ofilename, "w")) == NULL ) {
            fprintf(stderr, "Error opening file: %s", ofilename);
            exit(EXIT_FAILURE);
        }
    }

    fst_read(fst_a, fin_a);
    fst_read(fst_b, fin_b);

    fst_compose(fst_a, fst_b, fst_c);

    //fst_trim(fst_c);

    fst_write(fst_c, fout);

    fst_remove(fst_a);
    fst_remove(fst_b);
    fst_remove(fst_c);
    
    if (fin_a != stdout) {
        fclose(fin_a);
    }
    if (fin_b != stdout) {
        fclose(fin_b);
    }
    if (fout != stdout) {
        fclose(fout);
    }

    return 0;
}
