/*
 *  mixtest_lexer_helper.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIXTEST_LEXER_HELPER_H
#define MIXTEST_LEXER_HELPER_H

int string_nextchar(void);
int string_ungetchar(int);
void string_setup(const char *s);

#endif