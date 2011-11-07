//
//  mix_instr_io.c
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "mix_opcodes.h"
#include "mix_instr_io.h"


int mix_machine_instr_IOC(mix_machine *mix, int unit, int m, int c) {
    if (mix->devices[unit] == NULL) {
        return MIX_M_ERROR;
    }
    mix_device_control(mix->devices[unit], m);
    mix->time++;
    mix->ip++;
    return MIX_M_OK;
}

int mix_machine_instr_OUT(mix_machine *mix, int unit, int m, int c) {
    if (mix->devices[unit] == NULL) {
        return MIX_M_ERROR;
    }
    mix_device_printer_output(mix->devices[unit], &(mix->words[m]));
    mix->ip++;
    mix->time++;
    return MIX_M_OK;
}