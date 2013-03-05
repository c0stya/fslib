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
    const char * ofilename = NULL;
    /*
    char isym_filename[0xff];
    char osym_filename[0xff];
    char ssym_filename[0xff];
    */

    FILE * fin = stdin;
    FILE * fout = stdout;
    FILE * isym = NULL;
    FILE * osym = NULL;
    FILE * ssym = NULL;

    int c;
    int opt_index = -1;

    const char * short_options = "I:O:S:";

    const struct option long_options [] = {
        { "isym", required_argument, NULL, 'I' },
        { "osym", required_argument, NULL, 'O' },
        { "ssym", required_argument, NULL, 'S' },
        { 0, 0, NULL, 0}
    };

    while ( (c = getopt_long(argc, argv,  
                short_options, long_options, &opt_index )) != -1 ) {
        switch(c) {
            case 'I': 
                isym = file_open(optarg);
                break;
            case 'O': 
                osym = file_open(optarg);
                break;
            case 'S': 
                ssym = file_open(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [--isym filename] [--osym filename]"
                    "[--ssym filename] [input_file] [output_file]\n", argv[0]);
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

    /*
    if (isym == NULL && osym == NULL && ssym == NULL) 
        fst = fst_compile(fin);
    else
        fst = fst_compile_st(fin, isym, osym, ssym);
    */

    fst = fst_create();
    fst_compile(fst, fin);
    fst_write(fst, fout);
    fst_remove(fst);
    
    if (fin != stdout) {
        fclose(fin);
    }
    if (fout != stdout) {
        fclose(fout);
    }

    if (isym != NULL) {
        fclose(isym);
    }
    if (osym != NULL) {
        fclose(osym);
    }
    if (ssym != NULL) {
        fclose(ssym);
    }

    return 0;
}

