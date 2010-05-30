/*
 *  mixassemble.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIX_ASSEMBLE_H
#define MIX_ASSEMBLE_H

int mixas_encode(const char *opcode, int *c, int *f);

typedef struct ma_assembly ma_assembly;

ma_assembly *ma_assembly_create(void);

void ma_process_line(ma_assembly *, const char *);

int ma_get_current(ma_assembly *);

#endif