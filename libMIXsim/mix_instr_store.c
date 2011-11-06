//
//  mix_instr_store.c
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_opcodes.h"
#include "mix_instr_store.h"

int mix_machine_instr_STi (mix_machine *mix, int f, int m, int c) {
    int i = c - MIX_OP_STA;
    
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

