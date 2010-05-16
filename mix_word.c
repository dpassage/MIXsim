/*
 *  mix_word.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "mix_word.h"


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

void mix_word_set_sign(mix_word *w, int sign) {
	w->bytes[0] = sign;
}

void mix_word_set_byte(mix_word *w, int byte, int val) {
	w->bytes[byte] = val;
}

void mix_word_set(mix_word *w, const char *s) {
	char sign;
	sscanf(s, "%c %d %d %d %d %d", &sign, &(w->bytes[1]), &(w->bytes[2]), &(w->bytes[3]), &(w->bytes[4]), &(w->bytes[5]));
	w->bytes[0] = (sign == '-') ? MIX_WORD_MINUS : MIX_WORD_PLUS;
}

/* - xx xx xx xx xx */
char *mix_word_tostring(mix_word *w) {
	char *ret;
	char sign = (w->bytes[0] == MIX_WORD_PLUS) ? '+' : '-';
	
	asprintf(&ret, "%c %.2d %.2d %.2d %.2d %.2d", sign, w->bytes[1], w->bytes[2], w->bytes[3], w->bytes[4], w->bytes[5]);
	
	return ret;
}

void mix_word_clear(mix_word *w) {
	w->bytes[0] = MIX_WORD_PLUS;
	for (int i = 1; i <= 5; i++) {
		w->bytes[i] = 0;
	}
	return;
}
