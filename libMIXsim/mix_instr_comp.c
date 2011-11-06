//
//  mix_instr_comp.c
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_opcodes.h"
#include "mix_instr_comp.h"


int mix_machine_instr_CMPA(mix_machine *mix, int f, int m, int c) {
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
