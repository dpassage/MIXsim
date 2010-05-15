/*
 *  mix_assembler.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIX_ASSEMBLER_H
#define MIX_ASSEMBLER_H

#include "mix_word.h"

/**
 * Assemble a single line of MIX assembly into a word. 
 * Ignores leading and trailing whitespace
 * Caller responsible for providing pointer to valid word
 * Returns 0 on success, non-zero otherwise
 */
int mix_assemble_line(const char *l, mix_word *w);

#endif /* MIX_ASSEMBLER_H */