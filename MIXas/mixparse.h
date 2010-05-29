/*
 *  mixparse.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIXPARSE_H
#define MIXPARSE_H

#include <stdio.h>

void mixparse_reset(void);

void mixparse_set_input_file(FILE *);
void mixparse_set_input_string(const char *);

void mixparse_setcallback_comment(void (*callback)(void));
void mixparse_setcallback_address(void (*callback)(int));
void mixparse_setcallback_instruction(void (*callback)(const char *, int, int, int));

void mixparse_debug(void);

int mixparse(void);

void mixparse_comment(void);
void mixparse_address(int);
void mixparse_instruction(const char *, int, int, int);

#endif