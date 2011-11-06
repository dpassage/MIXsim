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
#define MIX_M_UNIMPLEMENTED -30
#define MIX_M_UNDEF -42

struct mix_machine {
	int ip;
	unsigned int time;
    int overflow;
    int comparison;
    mix_word ri[9]; /* 0 is ra; 7 is rx; 8 is rj */
	mix_word words[4000];
    mix_device *devices[20];
    
    void (*callback_exec)(int ip, const mix_word *w);
};

typedef struct mix_machine mix_machine;

mix_machine *mix_machine_create(void);
void mix_machine_destroy(mix_machine *m);

/* memory management */
void mix_machine_load_mem(mix_machine *m, const mix_word *w, int loc);
mix_word *mix_machine_read_mem(mix_machine *m, mix_word *w, int loc);

/* machine state */
int mix_machine_get_time(mix_machine *m);

void mix_machine_set_ip(mix_machine *m, int loc);
int mix_machine_get_ip(mix_machine *m);

int mix_machine_get_overflow(mix_machine *m);

#define MIX_M_LESS -1
#define MIX_M_EQUAL 0
#define MIX_M_GREATER 1

void mix_machine_set_comparison(mix_machine *m, int comp);
int mix_machine_get_comparison(mix_machine *m);

/* register management */
mix_word *mix_machine_read_ra(mix_machine *m, mix_word *w);
mix_word *mix_machine_read_ri(mix_machine *m, mix_word *w, int i);
mix_word *mix_machine_read_rx(mix_machine *m, mix_word *w);
mix_word *mix_machine_read_rj(mix_machine *m, mix_word *w);

void mix_machine_load_ra(mix_machine *m, const mix_word *w);
void mix_machine_load_ri(mix_machine *m, const mix_word *w, int i);
void mix_machine_load_rx(mix_machine *m, const mix_word *w);

/* device management */
void mix_machine_device_attach(mix_machine *m, mix_device *d, int unit);
mix_device *mix_machine_device_get(mix_machine *m, int unit);

/* callbacks */
void mix_machine_set_callback_exec(mix_machine *m, void (*callback_exec)(int ip, const mix_word *instr));

/* turn the crank! */
int mix_machine_execute(mix_machine *m);

typedef int(*mix_instruction)(mix_machine *mix, int f, int m, int c);

/* instructions in alpha order by mixal symbol */
int mix_machine_instr_CMPA(mix_machine *mix, int f, int m);
int mix_machine_instr_DIV (mix_machine *mix, int f, int m);
int mix_machine_instr_ENTi(mix_machine *mix, int f, int m, int i);
int mix_machine_instr_IOC (mix_machine *mix, int f, int m);
int mix_machine_instr_INCi(mix_machine *mix, int f, int m, int i);
int mix_machine_instr_Ji  (mix_machine *mix, int f, int m, int i);
int mix_machine_instr_LDi (mix_machine *mix, int f, int m, int i);
int mix_machine_instr_LDA (mix_machine *mix, int f, int m);
int mix_machine_instr_LDX (mix_machine *mix, int f, int m);
int mix_machine_instr_STi (mix_machine *mix, int f, int m, int i);

/* helper functions */
void mix_load_reg(mix_word *reg, const mix_word *mem, int f);

#endif /* MIX_MACHINE_H */