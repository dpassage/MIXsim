/*
 *  mixlex.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixlex.h"

#include "y.tab.h"

FILE *inputfile;
int lex_column = 0;
char symbolbuffer[11];
int (*getnextchar)(void);

void mixlex_input(FILE *f) {
    inputfile = f;
}
void mixlex_set_getchar(int (*nextchar)(void)) {
    getnextchar = nextchar;
}

int yylex (void) {
    
    char ch = (getnextchar)();
    if (lex_column == 0 && ch == '*') {
        /* this is a comment line */
        do {
            ch = (getnextchar)();
            if (ch == -1) {
                return -1;
            }
        } while (ch != '\n');
        return COMMENT;
    }
    return -1;
}
