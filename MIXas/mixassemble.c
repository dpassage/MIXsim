/*
 *  mixassemble.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <ctype.h>
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

static int ma_parse_opcode(char *opcode, const char *l) {   
    char const *codestart = &l[11];
    int i;
    
    for (i = 0; i <= 5; i++) {
        if (isupper(codestart[i]) || isdigit(codestart[i])) {
            opcode[i] = codestart[i];
        } else if (codestart[i] == ' ') {
            opcode[i] = '\0';
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

static int ma_parse_loc(char *loc, const char *l) {
    char const *codestart = l;
    int i;
    
    for (i = 0; i <= 11; i++) {
        if (isupper(codestart[i]) || isdigit(codestart[i])) {
            loc[i] = codestart[i];
        } else if (codestart[i] == ' ') {
            loc[i] = '\0';
            return 1;
        } else {
            return 0;
        }
    }
    return 0;    
}

typedef struct symbol_entry {
    int val;
    char symbol[11];
    struct symbol_entry *next;
} symbol_entry;

static symbol_entry *symbol_table = NULL;

int ma_get_symbol(ma_assembly *ma, int *val, const char *symbol) {
    symbol_entry *next;
    for (next = symbol_table; next != NULL; next = next->next) {
        if (strcmp(next->symbol, symbol) == 0) {
            *val = next->val;
            return 1;
        }
    }
    return 0;
}

int ma_set_symbol(ma_assembly *ma, const char *symbol, int value) {
    int throwaway;
    symbol_entry *new;
    if (ma_get_symbol(ma, &throwaway, symbol)) {
        return 0;
    }
    new = (symbol_entry *)malloc(sizeof(symbol_entry));
    new->val = value;
    strncpy(new->symbol, symbol, (size_t)11);
    new->next = symbol_table;
    symbol_table = new;
    return 1;
}

int ma_process_line(ma_assembly *ma, const char *l) {
    char loc[11];
    char opcode[5];
    
    if (strlen(l) < 15) {
        return 0;
    }
    int ret = ma_parse_opcode(&opcode[0], l);
    if (ret == 0) {
        return 0;
    }
    ret = ma_parse_loc(&loc[0], l);
    if (ret == 0) {
        return 0;
    }
    
    if (strcmp(opcode, "ORIG") == 0) {
        ma->current = atoi(l + 16);
        return 1;
    } else if (strcmp(opcode, "EQU") == 0) {
        if (loc[0] == '\0') {
            return 0;
        }
        int value = atoi(l + 16);
        return ma_set_symbol(ma, loc, value);
    }
    return 0;
}
