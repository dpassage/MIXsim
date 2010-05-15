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

typedef struct mix_word mix_word;

/* Returns a word data structure with all-0 contents
 * Caller responsible for destroying
 */
mix_word *mix_word_create(void);

/* Destroys a word data structure
 */
void mix_word_destroy(mix_word *w);

/* Converts a word data structure to standard string representation
 * Caller responsible for freeing data structure
 */
char *mix_word_tostring(mix_word *w);


#endif /* MIX_WORD_H */