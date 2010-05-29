/*
 *  mixparse.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixlex.h"
#include "mixparse.h"

extern int yyparse();

static void (*comment_callback)(void);
static void comment_callback_null(void) {
}

void mixparse_reset(void) {
    comment_callback = comment_callback_null;
    mixlex_reset();
}

void mixparse_setcallback_comment(void (*callback)(void)) {
    comment_callback = callback;
}

void mixparse_set_input_file(FILE *f) {
    mixlex_input(f);
}

void mixparse_set_input_string(char *s) {
    mixlex_input_string(s);
}

void mixparse_comment(void) {
    (*comment_callback)();
}
    
int mixparse(void) {
    return yyparse();
}
