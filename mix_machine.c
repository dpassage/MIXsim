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
#include "mix_opcodes.h"
#include "mix_word.h"

struct mix_machine {
	int ip;
	unsigned int time;
    mix_word ri[7]; /* only 1-6 used */
	mix_word ra;
	mix_word rx;
	mix_word words[4000];
    mix_device *devices[20];
};

mix_machine *mix_machine_create(void)
{
	return (mix_machine *)malloc(sizeof(mix_machine));
}

void mix_machine_destroy(mix_machine *m) {
    free(m);
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

void mix_machine_set_ip(mix_machine *m, int loc)
{
	m->ip = loc;
	return;
}

int mix_machine_get_ip(mix_machine *m)
{
	return m->ip;
}


int mix_machine_get_time(mix_machine *m) {
	return m->time;
}

mix_word *mix_machine_read_ri(mix_machine *m, mix_word *w, int i) {
    *w = m->ri[i];
    return w;
}
    
mix_word *mix_machine_read_r1(mix_machine *m, mix_word *w) {	
	return mix_machine_read_ri(m, w, 1);
}

mix_word *mix_machine_read_ra(mix_machine *m, mix_word *w) {
	*w = m->ra;
	return w;
}

mix_word *mix_machine_read_rx(mix_machine *m, mix_word *w) {
	*w = m->rx;
	return w;
}

void mix_load_reg(mix_word *reg, mix_word *mem, int f) {
	
	mix_word_clear(reg);
	
	int fieldleft = f / 8;
	int fieldright = f % 8;
	if (fieldleft == 0) {
		reg->bytes[0] = mem->bytes[0];
		fieldleft++;
	} else {
		reg->bytes[0] = MIX_WORD_PLUS;
	}
	
	/* (1:4) 1->2 2->3 3->4 4->5 */
	int tocopy = (fieldright - fieldleft) + 1;
	int tobyte = 5;
	int frombyte = fieldright;
	for (; tocopy > 0; tocopy--, tobyte--, frombyte--) {
		reg->bytes[tobyte] = mem->bytes[frombyte];
	}
	
	return;
}

mix_device *mix_machine_device_get(mix_machine *m, int unit) {
    return m->devices[unit];
}

void mix_machine_device_attach(mix_machine *m, mix_device *d, int unit) {
    m->devices[unit] = d;
}

int mix_machine_instr_HLT(mix_machine *mix, int f, int m) {
    mix->time++;
    return MIX_M_HALT;
}

int mix_machine_instr_IOC(mix_machine *mix, int unit, int m) {
    if (mix->devices[unit] == NULL) {
        return MIX_M_ERROR;
    }
    mix_device_control(mix->devices[unit], m);
    mix->time++;
    mix->ip++;
    return MIX_M_OK;
}

int mix_machine_instr_LDi(mix_machine *mix, int f, int m, int i) {
    mix_load_reg(&(mix->ri[i]), &(mix->words[m]), f);
    mix->ri[i].bytes[1] = 0;
    mix->ri[i].bytes[2] = 0;
    mix->ri[i].bytes[3] = 0;    
    mix->time = mix->time + 2;
    mix->ip++;
    return MIX_M_OK;    
}
int mix_machine_instr_LDA(mix_machine *mix, int f, int m) {
    mix_load_reg(&(mix->ra), &(mix->words[m]), f);
    mix->time = mix->time + 2;
    mix->ip++;
    return MIX_M_OK;
}

int mix_machine_instr_LDX(mix_machine *mix, int f, int m) {
    mix_load_reg(&(mix->rx), &(mix->words[m]), f);
    mix->time = mix->time + 2;
    mix->ip++;
    return MIX_M_OK;
}


int mix_machine_execute(mix_machine *mix)
{
	mix_word instr = mix->words[mix->ip];
	int opcode = instr.bytes[5];
	int m = (instr.bytes[1] * 100) + instr.bytes[2] * 
            (instr.bytes[0] == MIX_WORD_MINUS ? -1 : 1);
    int f = instr.bytes[4];
	
	switch (opcode) {
        case MIX_OP_05:
            switch (f) {
                case 2: /* HLT */
                    return mix_machine_instr_HLT(mix, f, m);
                    break;
                default:
                    return -1;
                    break;
            }
            break;
        case MIX_OP_LD1:
            return mix_machine_instr_LDi(mix, f, m, 1);
            break;
		case MIX_OP_LDA:
            return mix_machine_instr_LDA(mix, f, m);
			break;
		case MIX_OP_LDX:
            return mix_machine_instr_LDX(mix, f, m);
			break;
        case MIX_OP_IOC:
            return mix_machine_instr_IOC(mix, f, m);
            break;
		default:
            return -1;
			break;
	}
	return 0;
}


