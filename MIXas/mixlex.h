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
int yylex(void);

#endif /* MIXLEX_H */