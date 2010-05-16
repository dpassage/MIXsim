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
	mix_word ra;
	mix_word rx;
	mix_word words[4000];
    mix_device *devices[20];
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

void mix_machine_device_control(mix_machine *mix, int unit, int m) {
    mix_device_control(mix->devices[unit], m);
    mix->time++;
}

int mix_machine_execute(mix_machine *mix)
{
	mix_word instr = mix->words[mix->ip];
	int opcode = instr.bytes[5];
	int m;
	
	switch (opcode) {
        case MIX_OP_05:
            switch (instr.bytes[4]) {
                case 2: /* HLT */
                    mix->time++;
                    return 1;
                    break;
                default:
                    return -1;
                    break;
            }
            break;

		case MIX_OP_LDA:
			m = (instr.bytes[1] * 100) + instr.bytes[2];
			mix_load_reg(&(mix->ra), &(mix->words[m]), instr.bytes[4]);
			mix->time = mix->time + 2;
			mix->ip++;
			break;
		case MIX_OP_LDX:
			m = (instr.bytes[1] * 100) + instr.bytes[2];
			mix_load_reg(&(mix->rx), &(mix->words[m]), instr.bytes[4]);
			mix->time = mix->time + 2;
			mix->ip++;
			break;
        case MIX_OP_IOC:
            mix_machine_device_control(mix, instr.bytes[4], m);
            mix->ip++;
            break;
		default:
            return -1;
			break;
	}
	return 0;
}


