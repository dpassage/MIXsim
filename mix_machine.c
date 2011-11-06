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
#include "mix_machine.h"
#include "mix_opcodes.h"
#include "mix_word.h"

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

int mix_machine_instr_HLT(mix_machine *mix, int f, int m) {
    mix->time++;
    return MIX_M_HALT;
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

int mix_machine_instr_DIV(mix_machine *mix, int f, int m) {
    long dividend;
    long divisor = m;
    
    if (f == 6) {
        return MIX_M_UNIMPLEMENTED;
    }
    
    dividend = mix_word_value(&(mix->ri[0]), MIX_F(0,5));
    dividend = (dividend * 10000000000L) + 
        (long)(mix_word_value(&(mix->ri[7]), MIX_F(1,5)));
    divisor = (long)mix_word_value(&(mix->words[m]), MIX_F(0, 5));
    
    long quotient = dividend / divisor;
    long remainder = dividend % divisor;
    
    mix_word_set_value(&(mix->ri[0]), MIX_F(0,5), (int)quotient);
    mix_word_set_value(&(mix->ri[7]), MIX_F(0,5), (int)remainder);
    mix->overflow = 0;
    
    mix->time += 12;
    mix->ip++;
    return MIX_M_OK;
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
    mix_load_reg(&(mix->ri[0]), &(mix->words[m]), f);
    mix->time = mix->time + 2;
    mix->ip++;
    return MIX_M_OK;
}

int mix_machine_instr_LDX(mix_machine *mix, int f, int m) {
    mix_load_reg(&(mix->ri[7]), &(mix->words[m]), f);
    mix->time = mix->time + 2;
    mix->ip++;
    return MIX_M_OK;
}

int mix_machine_instr_INCi(mix_machine *mix, int f, int m, int i) {
    mix_word *reg = &(mix->ri[i]);
    int accum = mix_word_value(reg, 5 /* (0:5) */);
    accum += m;
    if (accum < -9999 || accum > 9999 ) {
        return MIX_M_UNDEF;
    }
    mix_word_set_value(reg, 5 /* 0:5 */, accum);
    mix->time++;
    mix->ip++;
    return MIX_M_OK;
}

int mix_machine_instr_STi (mix_machine *mix, int f, int m, int i) {
    mix_word *reg = &(mix->ri[i]);
    mix_word *mem = &(mix->words[m]);
    int left = f / 8;
    int right = f % 8;
    
    if (left == 0) {
        mem->bytes[0] = reg->bytes[0];
        left++;
    }
    
    for (int j = 5; right >= left; j--, right--) {
        mem->bytes[right] = reg->bytes[j];
    }
    
    mix->ip++;
    mix->time += 2;
    return MIX_M_OK;
}

int mix_machine_instr_JMP (mix_machine *mix, int f, int m) {
    mix_word_set_value(&(mix->ri[8]), MIX_F(0, 2), mix->ip + 1);
    mix->ip = m;
    mix->time++;
    return MIX_M_OK;
}

int mix_machine_instr_JG (mix_machine *mix, int f, int m) {
    if (mix->comparison == MIX_M_GREATER) {
        mix->ip = m;
    } else {
        mix->ip++;
    }
    mix->time++;
    return MIX_M_OK;
}

int mix_machine_instr_Ji (mix_machine *mix, int f, int m, int i) {
    switch (f) {
        case 0: /* N */
            if (mix_word_value(&(mix->ri[i]), MIX_F(0, 5)) < 0) {
                return mix_machine_instr_JMP(mix, f, m);
            }
            break;
        case 1: /* Z */
            if (mix_word_value(&(mix->ri[i]), MIX_F(0, 5)) == 0) {
                return mix_machine_instr_JMP(mix, f, m);
            }
            break;
        case 2: /* P */
            if (mix_word_value(&(mix->ri[i]), MIX_F(0, 5)) > 0) {
                return mix_machine_instr_JMP(mix, f, m);
            }
            break;
        case 3: /* NN */
            if (mix_word_value(&(mix->ri[i]), MIX_F(0, 5)) >= 0) {
                return mix_machine_instr_JMP(mix, f, m);
            }
            break;
        case 4: /* NZ */
            if (mix_word_value(&(mix->ri[i]), MIX_F(0, 5)) != 0) {
                return mix_machine_instr_JMP(mix, f, m);
            }
            break;
        case 5: /* NP */
            if (mix_word_value(&(mix->ri[i]), MIX_F(0, 5)) <= 0) {
                return mix_machine_instr_JMP(mix, f, m);
            }
            break;
        default:
            return MIX_M_ERROR;
            break;
    }
    mix->ip++;
    mix->time++;
    return MIX_M_OK;
}

int mix_machine_instr_ENTi(mix_machine *mix, int f, int m, int i) {
    mix_word_set_value(&(mix->ri[i]), MIX_F(0,5), m);
    mix->ri[i].bytes[1] = 0;
    mix->ri[i].bytes[2] = 0;
    mix->ri[i].bytes[3] = 0;
    mix->ip++;
    mix->time++;
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
	
	switch (opcode) {
        case MIX_OP_DIV:
            return mix_machine_instr_DIV(mix, f, m);
            break;
        case MIX_OP_05:
            switch (f) {
                case 0: /* NUM */
                case 1: /* CHAR */
                    return MIX_M_UNIMPLEMENTED;
                    break;
                case 2: /* HLT */
                    return mix_machine_instr_HLT(mix, f, m);
                    break;
                default:
                    return MIX_M_ERROR;
                    break;
            }
            break;
		case MIX_OP_LDA:
            return mix_machine_instr_LDA(mix, f, m);
			break;
        case MIX_OP_LD1:
        case MIX_OP_LD2:
        case MIX_OP_LD3:
        case MIX_OP_LD4:
        case MIX_OP_LD5:
        case MIX_OP_LD6:
            i = opcode - MIX_OP_LDA;
            return mix_machine_instr_LDi(mix, f, m, i);
            break;
		case MIX_OP_LDX:
            return mix_machine_instr_LDX(mix, f, m);
			break;
        case MIX_OP_ST1:
        case MIX_OP_ST2:
        case MIX_OP_ST3:
        case MIX_OP_ST4:
        case MIX_OP_ST5:
        case MIX_OP_ST6:
            i = opcode - MIX_OP_STA;
            return mix_machine_instr_STi(mix, f, m, i);
            break;
        case MIX_OP_IOC:
            return mix_machine_instr_IOC(mix, f, m);
            break;
        case MIX_OP_JUMPS:
            switch (f) {
                case 0:
                    return mix_machine_instr_JMP(mix, f, m);
                    break;
                case 6:
                    return mix_machine_instr_JG(mix, f, m);
                    break;
                default:
                    return MIX_M_UNIMPLEMENTED;
                    break;
            }
            break;
        case MIX_OP_J1:
        case MIX_OP_J2:
        case MIX_OP_J3:
        case MIX_OP_J4:
        case MIX_OP_J5:
        case MIX_OP_J6:
        case MIX_OP_JX:
            i = opcode - MIX_OP_JA;
            return mix_machine_instr_Ji(mix, f, m, i);
            break;
        case MIX_OP_ADRA:
        case MIX_OP_ADR1:
        case MIX_OP_ADR2:
        case MIX_OP_ADR3:
        case MIX_OP_ADR4:
        case MIX_OP_ADR5:
        case MIX_OP_ADR6:
        case MIX_OP_ADRX:
            i = opcode - MIX_OP_ADRA;
            switch (f) {
                case 0: /* INCi */
                    return mix_machine_instr_INCi(mix, f, m, i);
                    break;
                case 1: /* DECi */
                    return MIX_M_UNIMPLEMENTED;
                    break;
                case 2: /* ENTi */
                    return mix_machine_instr_ENTi(mix, f, m, i);
                    break;
                case 3: /* ENNi */
                    return MIX_M_UNIMPLEMENTED;
                    break;
                default:
                    return MIX_M_ERROR;
                    break;
            }
        case MIX_OP_CMPA:
            return mix_machine_instr_CMPA(mix, f, m);
            break;
		default:
            return MIX_M_UNIMPLEMENTED;
			break;
	}
}


