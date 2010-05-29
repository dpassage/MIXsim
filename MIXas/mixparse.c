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
extern int yydebug;

static void (*comment_callback)(void);
static void comment_callback_null(void) {
}

static void (*address_callback)(int);
static void address_callback_null(int i) {
}

static void (*instruction_callback)(const char *, int);
static void instruction_callback_null(const char *s, int i) {
}

void mixparse_reset(void) {
    comment_callback = comment_callback_null;
    address_callback = address_callback_null;
    instruction_callback = instruction_callback_null;
    mixlex_reset();
}

void mixparse_setcallback_comment(void (*callback)(void)) {
    comment_callback = callback;
}

void mixparse_setcallback_address(void (*callback)(int)) {
    address_callback = callback;
}

void mixparse_setcallback_instruction(void (*callback)(const char *, int)) {
    instruction_callback = callback;
}

void mixparse_set_input_file(FILE *f) {
    mixlex_input(f);
}

void mixparse_set_input_string(const char *s) {
    mixlex_input_string(s);
}

void mixparse_debug(void) {
    yydebug = 1;
}

void mixparse_comment(void) {
    (*comment_callback)();
}
    
void mixparse_address(int i) {
    (*address_callback)(i);
}

void mixparse_instruction(const char *s, int i) {
    (*instruction_callback)(s, i);
}

int mixparse(void) {
    return yyparse();
}
