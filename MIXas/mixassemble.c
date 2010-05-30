/*
 *  mixassemble.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#include <string.h>

#include "mixassemble.h"

struct opcode_lookup {
    char *opcode;
    int c;
    int f;
} opcode_lookup[] = {
    { "EQU",  -1, -1 },
    { "IN",   36, -1 },
    { "LDA",   8, -1 },
    { "NOP",   0,  0 },
    { "ORIG", -1, -1 },
    { NULL,    0,  0 }
};

int mixas_encode(const char *opcode, int *c, int *f) {
    for (int i = 0; opcode_lookup[i].opcode != NULL; i++) {
        if (strcmp(opcode, opcode_lookup[i].opcode) == 0) {
            *c = opcode_lookup[i].c;
            *f = opcode_lookup[i].f;
            return 1;
        }
    }
    return 0;
}

struct ma_assembly {
    int current;
};

ma_assembly *ma_assembly_create(void) {
    ma_assembly *ma = (ma_assembly *)malloc(sizeof (ma_assembly));
    ma->current = 0;
    return ma;
}

int ma_get_current(ma_assembly *ma) {
    return ma->current;
}
