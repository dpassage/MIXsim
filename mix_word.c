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
	int a0;
};

mix_word *mix_word_create(void) {
	return (mix_word *)malloc(sizeof(mix_word));
}
