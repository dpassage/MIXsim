//
//  mix_instr_specials.c
//  MIXsim
//
//  Created by David Paschich on 11/6/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_instr_specials.h"

int mix_machine_instr_specials(mix_machine *mix, int f, int m, int c) {
    switch (f) {
        case 0:
            return MIX_M_UNIMPLEMENTED;
        case 1:
            return mix_machine_instr_CHAR(mix, f, m);
        case 2:
            return mix_machine_instr_HLT(mix, f, m);
            break;
        default:
            return MIX_M_ERROR;
            break;
    }
}

int mix_machine_instr_HLT(mix_machine *mix, int f, int m) {
    mix->time++;
    return MIX_M_HALT;
}

int mix_machine_instr_CHAR(mix_machine *mix, int f, int m) {
    int value;
    mix_word ra;
    mix_word rx;
    int i;
    
    mix_machine_read_ra(mix, &ra);
    mix_machine_read_rx(mix, &rx);

    value = mix_word_value(&ra, MIX_F(1,5)); /* ignore sign */
    
    for (i = 5; i >= 1; i--) {
        rx.bytes[i] = 30 + (value % 10);
        value = value / 10;
    }
    for (i = 5; i >= 1; i--) {
        ra.bytes[i] = 30 + (value % 10);
        value = value / 10;
    }
    
    mix_machine_load_ra(mix, &ra);
    mix_machine_load_rx(mix, &rx);
    
    mix->ip++;
    mix->time++;
    
    return MIX_M_OK;
}
