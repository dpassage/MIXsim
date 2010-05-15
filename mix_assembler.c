/*
 *  mix_assembler.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <ctype.h>
#include <string.h>

#include "mix_assembler.h"

int mix_assemble_line(const char *l, mix_word *w) {
	mix_word_set_sign(w, MIX_WORD_PLUS);
	mix_word_set_byte(w, 1, 20);
	mix_word_set_byte(w, 2, 0);
	mix_word_set_byte(w, 3, 2);
	mix_word_set_byte(w, 4, 3);
	mix_word_set_byte(w, 5, 8);

	return 0;
}
