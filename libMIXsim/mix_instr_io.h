//
//  mix_instr_io.h
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef MIXsim_mix_instr_io_h
#define MIXsim_mix_instr_io_h

#include "mix_machine.h"

int mix_machine_instr_IOC (mix_machine *mix, int f, int m, int c);
int mix_machine_instr_OUT (mix_machine *mix, int f, int m, int c);

#endif
