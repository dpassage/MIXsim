/*
 *  mix_machine.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>

#include "mix_machine.h"
#include "mix_word.h"

struct mix_machine {
	mix_word words[4000];
};

mix_machine *mix_machine_create(void)
{
	return (mix_machine *)malloc(sizeof(mix_machine));
}

void mix_machine_load_mem(mix_machine *m, mix_word *w, int loc)
{
	m->words[loc] = *w;
	return;
}

mix_word *mix_machine_read_mem(mix_machine *m, mix_word *w, int loc)
{
	*w = m->words[loc];
	return w;
}
