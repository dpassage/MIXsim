//
//  mix_instr_specials.c
//  MIXsim
//
//  Created by David Paschich on 11/6/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_instr_specials.h"

int mix_machine_instr_specials(mix_machine *mix, int f, int m) {
    switch (f) {
        case 0:
            return mix_machine_instr_HLT(mix, f, m);
            break;
        default:
            return MIX_M_UNIMPLEMENTED;
            break;
    }
}
int mix_machine_instr_HLT(mix_machine *mix, int f, int m) {
    mix->time++;
    return MIX_M_HALT;
}
