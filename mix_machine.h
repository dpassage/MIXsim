/*
 *  mix_machine.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIX_MACHINE_H
#define MIX_MACHINE_H

#include "mix_word.h"

typedef struct mix_machine mix_machine;

mix_machine *mix_machine_create(void);
void mix_machine_load_mem(mix_machine *m, mix_word *w, int loc);
mix_word *mix_machine_read_mem(mix_machine *m, mix_word *w, int loc);


#endif /* MIX_MACHINE_H */