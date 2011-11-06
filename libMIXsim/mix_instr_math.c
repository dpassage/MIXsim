//
//  mix_instr_math.c
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_instr_math.h"

int mix_machine_instr_DIV(mix_machine *mix, int f, int m, int c) {
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
