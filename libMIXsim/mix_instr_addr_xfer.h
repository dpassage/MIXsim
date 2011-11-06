//
//  mix_instr_addr_xfer.h
//  MIXsim
//
//  Created by David Paschich on 11/5/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef MIXsim_mix_instr_addr_xfer_h
#define MIXsim_mix_instr_addr_xfer_h

#include "mix_machine.h"

int mix_machine_instr_addr_xfer(mix_machine *mix, int f, int m, int c);
int mix_machine_instr_ENTi(mix_machine *mix, int f, int m, int i);
int mix_machine_instr_INCi(mix_machine *mix, int f, int m, int i);

#endif
