//
//  mix_instr_jumps.h
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef MIXsim_mix_instr_jumps_h
#define MIXsim_mix_instr_jumps_h

#include "mix_machine.h"

int mix_machine_instr_jumps(mix_machine *mix, int f, int m);
int mix_machine_instr_JG  (mix_machine *mix, int f, int m);
int mix_machine_instr_JMP (mix_machine *mix, int f, int m);

#endif
