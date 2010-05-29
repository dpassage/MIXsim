/*
 *  mixtest_lexer_helper.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixtest_lexer_helper.h"

static int cur = 0;
static char const *inputstring;
static char ungetbuffer[10];
static int ungetcur = 0;

int string_nextchar(void) {
    if (ungetcur > 0) {
        return ungetbuffer[--ungetcur];
    }
    if (inputstring[cur] == '\0') {
        return -1;
    }
    return inputstring[cur++];
}

int string_ungetchar(int ch) {
    if (ungetcur == 10) {
        return -1;
    }
    ungetbuffer[ungetcur] = ch;
    ungetcur++;
    return ch;
}

void string_setup(const char *s) {
    inputstring = s;
    cur = 0;
}
