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

#include "mix_word.h"

int mixas_encode(const char *opcode, int *c, int *f);

typedef struct ma_assembly ma_assembly;

ma_assembly *ma_assembly_create(void);

int ma_process_line(ma_assembly *, const char *);

int ma_get_current(ma_assembly *);
int ma_get_symbol(ma_assembly *, int *, const char *);
int ma_set_symbol(ma_assembly *, const char *, int);
int ma_get_word(ma_assembly *, mix_word *, int);
int ma_set_word(ma_assembly *, const mix_word *, int);

#define MA_OK 1
#define MA_ERROR -1
#define MA_DONE 2

#endif