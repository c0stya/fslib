#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "fst.h"

int main(int argc, char * argv[]) {
    struct _fst * fst = fst_create(); 
    const char * ifilename = NULL; 
    const char * ofilename = NULL;
    FILE * fin = stdin;
    FILE * fout = stdout;
    int sort_outer = 0;
    int c;

    while ( (c = getopt(argc, argv, "ioh")) != -1 ) {
        switch(c) {
            case 'i': 
                sort_outer = 0;
                break;
            case 'o': 
                sort_outer = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-o] [input_file] [output_file]\n", argv[0]);
                exit(EXIT_SUCCESS);
        } 
    }

    if (optind < argc)
        ifilename = argv[optind];
    if (optind+1 < argc)
        ofilename = argv[optind+1];
    
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
    fst_arc_sort(fst, sort_outer);
    fst_write(fst, fout);
    fst_remove(fst);
    
    if (fin != stdout) {
        fclose(fin);
    }

    if (fout != stdout) {
        fclose(fout);
    }

    return 0;
}
