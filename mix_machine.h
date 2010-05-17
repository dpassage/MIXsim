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

#include "mix_device.h"
#include "mix_word.h"

#define MIX_M_OK     0
#define MIX_M_HALT   1
#define MIX_M_ERROR -1
#define MIX_M_UNDEF -42

typedef struct mix_machine mix_machine;

mix_machine *mix_machine_create(void);
void mix_machine_destroy(mix_machine *m);

void mix_machine_load_mem(mix_machine *m, mix_word *w, int loc);
mix_word *mix_machine_read_mem(mix_machine *m, mix_word *w, int loc);
int mix_machine_get_time(mix_machine *m);
void mix_machine_set_ip(mix_machine *m, int loc);
int mix_machine_get_ip(mix_machine *m);
int mix_machine_execute(mix_machine *m);
mix_word *mix_machine_read_ri(mix_machine *m, mix_word *w, int i);
mix_word *mix_machine_read_ra(mix_machine *m, mix_word *w);
mix_word *mix_machine_read_rx(mix_machine *m, mix_word *w);

void mix_machine_device_attach(mix_machine *m, mix_device *d, int unit);
mix_device *mix_machine_device_get(mix_machine *m, int unit);

void mix_load_reg(mix_word *reg, mix_word *mem, int f);

/* opcode instructions in alpha order*/
int mix_machine_instr_HLT (mix_machine *mix, int f, int m);
int mix_machine_instr_IOC (mix_machine *mix, int f, int m);
int mix_machine_instr_INCi(mix_machine *mix, int f, int m, int i);
int mix_machine_instr_LDi (mix_machine *mix, int f, int m, int i);
int mix_machine_instr_LDA (mix_machine *mix, int f, int m);
int mix_machine_instr_LDX (mix_machine *mix, int f, int m);


#endif /* MIX_MACHINE_H */