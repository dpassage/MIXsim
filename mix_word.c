/*
 *  mix_word.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>

#include "mix_word.h"

struct mix_word {
	int bytes[6];
};

mix_word *mix_word_create(void) {
	int i;
	mix_word *w = (mix_word *)malloc(sizeof(mix_word));
	
	w->bytes[0] = MIX_WORD_PLUS;
	for (i = 1; i <= 5; i++) {
		w->bytes[i] = 0;
	}
	
	return w;
}

void mix_word_destroy(mix_word *w) {
	free(w);
}

/* return the sign of the mix word */
int mix_word_sign(mix_word *w) {
	return w->bytes[0];
}

/* return the indicated byte of the word */
int mix_word_byte(mix_word *w, int b) {
	return w->bytes[b];
}

char *mix_word_tostring(mix_word *w) {
	return NULL;
}
