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
    FILE * isym = NULL;
    FILE * osym = NULL;
    FILE * ssym = NULL;

    struct _symt * ist = NULL;
    struct _symt * ost = NULL;
    struct _symt * sst = NULL;

    int c;
    int opt_index = -1;

    const char * short_options = "h";

    const struct option long_options [] = {
        { "isym", required_argument, NULL, 'I' },
        { "osym", required_argument, NULL, 'O' },
        { "ssym", required_argument, NULL, 'S' },
        { "help", required_argument, NULL, 'h' },
        { 0, 0, NULL, 0}
    };

    while ( (c = getopt_long(argc, argv,  
                short_options, long_options, &opt_index )) != -1 ) {
        switch(c) {
            case 'I': 
                isym = file_open(optarg);
                ist = symt_create();
                symt_read(ist, isym);
                break;
            case 'O': 
                osym = file_open(optarg);
                ost = symt_create();
                symt_read(ost, osym);
                break;
            case 'S': 
                ssym = file_open(optarg);
                sst = symt_create();
                symt_read(sst, ssym);
                break;
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
    fst_read(fst, fin);
    fst_print_sym(fst, ist, ost, sst);
    fst_remove(fst);
    
    if (fin != stdout) {
        fclose(fin);
    }

    if (isym != NULL)
        fclose(isym);
    if (osym != NULL)
        fclose(osym);
    if (ssym != NULL)
        fclose(ssym);

    if (ist != NULL)
        symt_remove(ist); 
    if (ost != NULL)
        symt_remove(ost); 
    if (sst != NULL)
        symt_remove(sst); 

    return 0;
}
