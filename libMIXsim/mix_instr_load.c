//
//  mix_instr_load.c
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_opcodes.h"
#include "mix_instr_load.h"

int mix_machine_instr_LDA(mix_machine *mix, int f, int m, int c) {
    mix_load_reg(&(mix->ri[0]), &(mix->words[m]), f);
    mix->time = mix->time + 2;
    mix->ip++;
    return MIX_M_OK;
}

int mix_machine_instr_LDX(mix_machine *mix, int f, int m, int c) {
    mix_load_reg(&(mix->ri[7]), &(mix->words[m]), f);
    mix->time = mix->time + 2;
    mix->ip++;
    return MIX_M_OK;
}

int mix_machine_instr_LDi(mix_machine *mix, int f, int m, int c) {
    int i = c - MIX_OP_LDA;
    mix_load_reg(&(mix->ri[i]), &(mix->words[m]), f);
    mix->ri[i].bytes[1] = 0;
    mix->ri[i].bytes[2] = 0;
    mix->ri[i].bytes[3] = 0;    
    mix->time = mix->time + 2;
    mix->ip++;
    return MIX_M_OK;    
}

