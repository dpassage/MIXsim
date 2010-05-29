/*
 *  MIXas.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/25/10.
 *  Copyright 2010 David Paschich. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mixlex.h"
#include "mixparse.h"

int main (int argc, char * const argv[]) {
    FILE *input;
    FILE *output = stdout;
    
    int ch;
    
    while ((ch = getopt(argc, argv, "o:")) != -1) {
        switch (ch) {
            case 'o':
                output = fopen(optarg, "w");
                if (output == NULL) {
                    fprintf(stderr, "Could not open %s for output", optarg);
                    exit(-1);
                }
                break;
            default:
                break;
        }
    }
    argc -= optind;
    argv += optind;
    
    if (argv[0] != NULL) {
        input = fopen(argv[0], "r");
        if (input == NULL) {
            fprintf(stderr, "Could not open %s for input", argv[0]);
            exit(-1);
        }
    }
    
    mixparse_reset();
    mixparse_set_input_file(input);
    return mixparse();
    
}
