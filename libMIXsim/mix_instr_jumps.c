//
//  mix_instr_jumps.c
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_opcodes.h"
#include "mix_instr_jumps.h"

int mix_machine_instr_jumps(mix_machine *mix, int f, int m, int c) {
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

int mix_machine_instr_JMP (mix_machine *mix, int f, int m) {
    mix_word_set_value(&(mix->ri[8]), MIX_F(0, 2), mix->ip + 1);
    mix->ip = m;
    mix->time++;
    return MIX_M_OK;
}

int mix_machine_instr_Ji (mix_machine *mix, int f, int m, int c) {
    int i = c - MIX_OP_JA;
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
