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
    mix_word ri[7]; /* only 1-6 used */
	mix_word ra;
	mix_word rx;
    mix_word rj;
	mix_word words[4000];
    mix_device *devices[20];
};

mix_machine *mix_machine_create(void)
{
    mix_machine *m = (mix_machine *)malloc(sizeof(mix_machine));
    mix_word_clear(&(m->ra));
    mix_word_clear(&(m->rx));
    for (int i = 1; i <= 6; i++) {
        mix_word_clear(&(m->ri[i]));
    }
    return m;
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

void mix_machine_load_ri(mix_machine *m, mix_word *w, int i) {
    m->ri[i] = *w;
    m->ri[i].bytes[1] = 0;
    m->ri[i].bytes[2] = 0;
    m->ri[i].bytes[3] = 0;
}

mix_word *mix_machine_read_ra(mix_machine *m, mix_word *w) {
	*w = m->ra;
	return w;
}

mix_word *mix_machine_read_rx(mix_machine *m, mix_word *w) {
	*w = m->rx;
	return w;
}

mix_word *mix_machine_read_rj(mix_machine *m, mix_word *w) {
    *w = m->rj;
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
    mix_word_set_value(&(mix->rj), MIX_F(0, 2), mix->ip + 1);
    mix->ip = m;
    mix->time++;
    return MIX_M_OK;
}

int mix_machine_execute(mix_machine *mix)
{
	mix_word instr = mix->words[mix->ip];
    char mix_instr_text[100];

    printf("Running instr %d: ", mix->ip);
    if (mix_instr_decode(&instr, mix_instr_text) == NULL) {
        printf("Could not decode instruction %s\n", mix_word_tostring(&instr)); 
    } else {
        printf("%s\n", mix_instr_text); 
    }
    
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
        case MIX_OP_ADR1:
        case MIX_OP_ADR2:
        case MIX_OP_ADR3:
        case MIX_OP_ADR4:
        case MIX_OP_ADR5:
        case MIX_OP_ADR6:
            i = opcode - MIX_OP_ADRA;
            switch (f) {
                case 0: /* INCi */
                    return mix_machine_instr_INCi(mix, f, m, i);
                    break;
                default:
                    return MIX_M_ERROR;
                    break;
            }
		default:
            return -1;
			break;
	}
	return 0;
}


