//
//  mix_instr_addr_xfer.c
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_opcodes.h"
#include "mix_instr_addr_xfer.h"

int mix_machine_instr_addr_xfer(mix_machine *mix, int f, int m, int c) {
    int i = c - MIX_OP_ADRA;
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

int mix_machine_instr_ENTi(mix_machine *mix, int f, int m, int i) {
    mix_word_set_value(&(mix->ri[i]), MIX_F(0,5), m);
    mix->ri[i].bytes[1] = 0;
    mix->ri[i].bytes[2] = 0;
    mix->ri[i].bytes[3] = 0;
    mix->ip++;
    mix->time++;
    return MIX_M_OK;
}
