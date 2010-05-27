/*
 *  mixlex.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIXLEX_H
#define MIXLEX_H

#include <stdio.h>

void mixlex_input(FILE *);
void mixlex_set_getchar(int (*nextchar)(void));
void mixlex_reset(void);
char *mixlex_get_token(void);

int yylex(void);

#endif /* MIXLEX_H */