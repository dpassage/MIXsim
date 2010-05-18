/*
 *  mix_word.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIX_WORD_H
#define MIX_WORD_H

struct mix_word {
	int bytes[6];
};

typedef struct mix_word mix_word;

#define MIX_WORD_PLUS (1)
#define MIX_WORD_MINUS (-1)

#define MIX_F(l, r) (8*(l)+(r))

/* 
 * Returns a word data structure with all-0 contents
 * Caller responsible for destroying
 */
mix_word *mix_word_create(void);

/* Destroys a word data structure */
void mix_word_destroy(mix_word *w);

/* return the sign of the mix word */
int mix_word_sign(mix_word *w);

/* return the indicated byte of the word */
int mix_word_byte(mix_word *w, int b);

void mix_word_set_sign(mix_word *w, int sign);
void mix_word_set_byte(mix_word *w, int byte, int val);
void mix_word_set(mix_word *w, const char *s);
void mix_word_set_value(mix_word *w, int f, int val);
void mix_word_clear(mix_word *w);

/* Returns the integer value of the indicated fields */
int mix_word_value(mix_word *w, int f);

/* 
 * Converts a word data structure to standard string representation
 * Caller responsible for freeing data structure
 */
char *mix_word_tostring(mix_word *w);


#endif /* MIX_WORD_H */