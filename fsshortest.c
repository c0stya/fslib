#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "fst.h"

FILE * file_open(const char * filename) { 
    FILE * s;
    if (( s = fopen(filename, "r")) == NULL ) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return s;
}
 

int main(int argc, char * argv[]) {
    struct _fst * fst; 
    const char * ifilename = NULL; 

    FILE * fin = stdin;

    int c;
    int opt_index = -1;

    const char * short_options = "h";

    const struct option long_options [] = {
        { "help", required_argument, NULL, 'h' },
        { 0, 0, NULL, 0}
    };

    while ( (c = getopt_long(argc, argv,  
                short_options, long_options, &opt_index )) != -1 ) {
        switch(c) {
            default:
                fprintf(stderr, "Usage: %s [input_file]\n", argv[0]);
                exit(EXIT_SUCCESS);
        } 
    }

    if (optind < argc)
        ifilename = argv[optind];
    
    if (ifilename) {
        if (( fin = fopen(ifilename, "rb")) == NULL ) {
            fprintf(stderr, "Error opening file: %s", ifilename);
            exit(EXIT_FAILURE);
        }
    }

    fst = fst_create();
    path = fst_create();

    fst_read(fst, fin);
    fst_shortest(fst, path);

    fst_remove(fst);
    fst_remove(path);
    
    if (fin != stdout) {
        fclose(fin);
    }

    return 0;
}

