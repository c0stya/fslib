#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "fst.h"
#include "symt.h"

FILE * file_open(const char * filename) { 
    FILE * s;
    if (( s = fopen(filename, "r")) == NULL ) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return s;
}
 

int main(int argc, char * argv[]) {
    struct _fst * fst = fst_create(); 

    const char * ifilename = NULL; 
    const char * ofilename = NULL;

    FILE * fin = stdin;
    FILE * fout = stdout;
    FILE * isym = NULL;
    FILE * osym = NULL;
    FILE * ssym = NULL;

    struct _symt * ist = NULL;
    struct _symt * ost = NULL;
    struct _symt * sst = NULL;

    int c;
    int acc=0;
    int opt_index = -1;

    const char * short_options = "I:O:S:";

    const struct option long_options [] = {
        { "isym", required_argument, NULL, 'I' },
        { "osym", required_argument, NULL, 'O' },
        { "ssym", required_argument, NULL, 'S' },
        { "acc", 0, NULL, 'a' },
        { "help", 0, NULL, 'h' },
        { "sr", 0, NULL, 's' },
        { 0, 0, NULL, 0}
    };

    while ( (c = getopt_long(argc, argv,  
                short_options, long_options, &opt_index )) != -1 ) {
        switch(c) {
            case 'a':
                acc = 1;
                break;
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
                fprintf(stderr, "Usage: %s [--acc] [--isym filename] [--osym filename]"
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

    fst = fst_compile(fst, fin, ist, ost, sst, acc);
    fst_write(fst, fout);

    fst_remove(fst);
    
    if (fin != stdout)
        fclose(fin);
    if (fout != stdout)
        fclose(fout);

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

