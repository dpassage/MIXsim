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

#include "mixassemble.h"

static int mix_assemble(FILE *input, FILE *output) {
    char buf[100];
    int line = 1;
    ma_assembly *ma = ma_assembly_create();
    
    while (fgets(buf, 100, input)) {
        if (ma_process_line(ma, buf) == MA_ERROR) {
            fprintf(stderr, "%d: could not process line\n", line);
            return -1;
        }
        line++;
    }
    
    return -1;
}

int main (int argc, char * const argv[]) {
    FILE *input = stdin;
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
    argv += optind;
    
    if (argv[0] != NULL) {
        input = fopen(argv[0], "r");
        if (input == NULL) {
            fprintf(stderr, "Could not open %s for input", argv[0]);
            exit(-1);
        }
    }
    
    return mix_assemble(input, output);
}

