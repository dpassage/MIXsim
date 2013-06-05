/*
 *  mix_instr_decode.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIX_INSTR_DECODE
#define MIX_INSTR_DECODE

#include "mix_machine.h"
#include "mix_word.h"

char *mix_instr_decode(const mix_word *w, char *buffer);

mix_instruction mix_instruction_lookup(int c);

#endif /* MIX_INSTR_DECODE */
