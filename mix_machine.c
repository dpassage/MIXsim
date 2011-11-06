/*
 *  mix_machine.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "mix_instr_decode.h"
#include "mix_instr_jumps.h"
#include "mix_machine.h"
#include "mix_opcodes.h"
#include "mix_word.h"


static void null_callback_exec(int ip, const mix_word *w) {
    return;
}
                               
mix_machine *mix_machine_create(void)
{
    mix_machine *m = (mix_machine *)malloc(sizeof(mix_machine));

    for (int i = 0; i <= 8; i++) {
        mix_word_clear(&(m->ri[i]));
    }
    
    m->callback_exec = null_callback_exec;
    
    return m;
}

void mix_machine_destroy(mix_machine *m) {
    free(m);
}

void mix_machine_load_mem(mix_machine *m, const mix_word *w, int loc) {
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

int mix_machine_get_overflow(mix_machine *m)
{
    return m->overflow;
}

void mix_machine_set_comparison(mix_machine *m, int i) 
{
    m->comparison = i;
}

int mix_machine_get_comparison(mix_machine *m)
{
    return m->comparison;
}

int mix_machine_get_time(mix_machine *m) {
	return m->time;
}

void mix_machine_load_ra(mix_machine *m, const mix_word *w) {
    m->ri[0] = *w;
}

void mix_machine_load_ri(mix_machine *m, const mix_word *w, int i) {
    m->ri[i] = *w;
    m->ri[i].bytes[1] = 0;
    m->ri[i].bytes[2] = 0;
    m->ri[i].bytes[3] = 0;
}

void mix_machine_load_rx(mix_machine *m, const mix_word *w) {
    m->ri[7] = *w;
}

mix_word *mix_machine_read_ra(mix_machine *m, mix_word *w) {
	*w = m->ri[0];
	return w;
}

mix_word *mix_machine_read_ri(mix_machine *m, mix_word *w, int i) {
    *w = m->ri[i];
    return w;
}

mix_word *mix_machine_read_rx(mix_machine *m, mix_word *w) {
	*w = m->ri[7];
	return w;
}

mix_word *mix_machine_read_rj(mix_machine *m, mix_word *w) {
    *w = m->ri[8];
    return w;
}

void mix_load_reg(mix_word *reg, const mix_word *mem, int f) {
	
	mix_word_clear(reg);
	
	int fieldleft = f / 8;
	int fieldright = f % 8;
	if (fieldleft == 0) {
		reg->bytes[0] = mem->bytes[0];
		fieldleft++;
	} else {
		reg->bytes[0] = MIX_WORD_PLUS;
	}
	
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


int mix_machine_instr_CMPA(mix_machine *mix, int f, int m) {
    int left = mix_word_value(&(mix->ri[0]), f);
    int right = mix_word_value(&(mix->words[m]), f);
    
    if (left < right)
        mix->comparison = MIX_M_LESS;
    else if (left == right)
        mix->comparison = MIX_M_EQUAL;
    else 
        mix->comparison = MIX_M_GREATER;
    
    mix->ip++;
    mix->time += 2;
    return MIX_M_OK;
}




void mix_machine_set_callback_exec(mix_machine *mix, void (*callback_exec)(int ip, const mix_word *instr)) {
    mix->callback_exec = callback_exec;
}

int mix_machine_execute(mix_machine *mix)
{
	mix_word instr = mix->words[mix->ip];

    (mix->callback_exec)(mix->ip, &instr);
        
	int opcode = mix_word_value(&instr, 45 /* (5:5) */);
	int m =      mix_word_value(&instr,  2 /* (0:2) */);
    int f =      mix_word_value(&instr, 36 /* (4:4) */);
    int i =      mix_word_value(&instr, 27 /* (3:3) */);
    
    if (i > 6) {
        return MIX_M_ERROR;
    }
    if (i > 0) {
        m += mix_word_value(&(mix->ri[i]), 5 /* (0:5) */);
    }
	
    mix_instruction instruction_pointer;
	switch (opcode) {
        case MIX_OP_DIV:
        case MIX_OP_05:
        case MIX_OP_LDA:
        case MIX_OP_LD1:
        case MIX_OP_LD2:
        case MIX_OP_LD3:
        case MIX_OP_LD4:
        case MIX_OP_LD5:
        case MIX_OP_LD6:
        case MIX_OP_LDX:
        case MIX_OP_ST1:
        case MIX_OP_ST2:
        case MIX_OP_ST3:
        case MIX_OP_ST4:
        case MIX_OP_ST5:
        case MIX_OP_ST6:
        case MIX_OP_IOC:
        case MIX_OP_JUMPS:
        case MIX_OP_J1:
        case MIX_OP_J2:
        case MIX_OP_J3:
        case MIX_OP_J4:
        case MIX_OP_J5:
        case MIX_OP_J6:
        case MIX_OP_JX:
        case MIX_OP_ADRA:
        case MIX_OP_ADR1:
        case MIX_OP_ADR2:
        case MIX_OP_ADR3:
        case MIX_OP_ADR4:
        case MIX_OP_ADR5:
        case MIX_OP_ADR6:
        case MIX_OP_ADRX:
            instruction_pointer = mix_instruction_lookup(opcode);
            return (*instruction_pointer)(mix, f, m, opcode);
            break;
        case MIX_OP_CMPA:
            return mix_machine_instr_CMPA(mix, f, m);
            break;
		default:
            return MIX_M_UNIMPLEMENTED;
			break;
	}
}


