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
int mix_machine_get_time(mix_machine *m);
void mix_machine_set_ip(mix_machine *m, int loc);
int mix_machine_get_ip(mix_machine *m);
void mix_machine_execute(mix_machine *m);
mix_word *mix_machine_read_ra(mix_machine *m, mix_word *w);
mix_word *mix_machine_read_rx(mix_machine *m, mix_word *w);
void mix_load_reg(mix_word *reg, mix_word *mem, int f);


#endif /* MIX_MACHINE_H */