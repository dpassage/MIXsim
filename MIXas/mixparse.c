/*
 *  mixparse.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixparse.h"

extern int yyparse();

static void (*comment_callback)(void);
static void comment_callback_null(void) {
}

void mixparse_reset(void) {
    comment_callback = comment_callback_null;
}

void mixparse_setcallback_comment(void (*callback)(void)) {
    comment_callback = callback;
}

void mixparse_comment(void) {
    (*comment_callback)();
}
    
int mixparse(void) {
    return yyparse();
}
