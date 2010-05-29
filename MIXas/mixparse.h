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
void mixparse_set_input_string(char *);

void mixparse_setcallback_comment(void (*callback)(void));

int mixparse(void);

void mixparse_comment(void);

#endif