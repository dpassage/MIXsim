/*
 *  mix_op_decode.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>

#include "mix_opcodes.h"
#include "mix_instr_decode.h"

static const char regtrans[] = {
    'A',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    'X'
};

static char *opcode_05(char *, int, int);
static char *opcode_addr_trans(char *, int, int);
static char *opcode_jreg_trans(char *, int, int);

static char *field_omitted(char *, int, int); /* field is usually omitted */
static char *field_standard(char *, int, int); /* field in standard (l:r) notation */
static char *field_decimal(char *, int, int); /* field in decimal notation */

static char *opcode_05(char *buf, int f, int c) {
    char *ans;
    switch (f) {
        case 0:
            ans = "NUM  ";
            break;
        case 1:
            ans = "CHAR ";
            break;
        case 2:
            ans = "HLT  ";
            break;
        default:
            return NULL;
            break;
    }
    strncpy(buf, ans, (size_t)6);
    return buf;
}

static char *opcode_addr_trans(char *buf, int f, int c) {
    char *base;
    char reg;
    switch (f) {
        case 0:
            base = "INC";
            break;
        case 1:
            base = "DEC";
            break;
        case 2:
            base = "ENT";
            break;
        case 3:
            base = "ENN";
            break;
        default:
            return NULL;
            break;
    }
    
    switch (c - 48) {
        case 0:
            reg = 'A';
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            reg = '0' + (c - 48);
            break;
        case 7:
            reg = 'X';
            break;
        default:
            return NULL;
            break;
    }
    
    sprintf (buf, "%s%c ", base, reg);
    return buf;
}

static char *opcode_jreg_trans(char *buf, int f, int c) {
    buf[0] = 'J';
    buf[1] = regtrans[c - MIX_OP_JA];
    switch (f) {
        case 0:
            buf[2] = 'N';
            buf[3] = ' ';
            break;
        case 1:
            buf[2] = 'Z';
            buf[3] = ' ';
            break;
        default:
            return NULL;
            break;
    }
    buf[4] = ' ';
    buf[5] = '\0';
    return buf;
}

static char* field_omitted(char *buf, int f, int c) {
    buf[0] = '\0';
    return buf;
}

static char* field_standard(char *buf, int f, int c) {
    int l = f / 8;
    int r = f % 8;
    if (l > 5 || r > 5) {
        return NULL;
    }
    sprintf(buf, "(%d:%d)", l, r);
    return buf;
}

static char* field_decimal(char *buf, int f, int c) {
    sprintf(buf, "(%d)", f);
    return buf;
}

struct mix_decoding_struct {
    char *opstr;
    char *(*opcode)(char *, int, int);
    char *(*field)(char *, int, int);
} mix_decoding_table[] = {
    /*00*/  { "NOP  ", 0, field_omitted },
    /*01*/  { 0, 0, 0 },
    /*02*/  { 0, 0, 0 },
    /*03*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*05*/  { 0, opcode_05, field_omitted },
    /*06*/  { 0, 0, 0 },
    /*07*/  { 0, 0, 0 },
    /*08*/  { "LDA  ", 0, field_standard },
    /*09*/  { "LD1  ", 0, field_standard },
    /*10*/  { "LD2  ", 0, field_standard },
    /*11*/  { 0, 0, 0 },
    /*12*/  { 0, 0, 0 },
    /*13*/  { 0, 0, 0 },
    /*14*/  { 0, 0, 0 },
    /*15*/  { 0, 0, 0 },
    /*16*/  { 0, 0, 0 },
    /*17*/  { 0, 0, 0 },
    /*18*/  { 0, 0, 0 },
    /*19*/  { 0, 0, 0 },
    /*20*/  { 0, 0, 0 },
    /*21*/  { 0, 0, 0 },
    /*22*/  { 0, 0, 0 },
    /*23*/  { 0, 0, 0 },
    /*24*/  { 0, 0, 0 },
    /*25*/  { 0, 0, 0 },
    /*26*/  { "ST2  ", 0, field_standard },
    /*27*/  { 0, 0, 0 },
    /*28*/  { 0, 0, 0 },
    /*29*/  { 0, 0, 0 },
    /*30*/  { 0, 0, 0 },
    /*31*/  { 0, 0, 0 },
    /*32*/  { 0, 0, 0 },
    /*33*/  { 0, 0, 0 },
    /*34*/  { 0, 0, 0 },
    /*35*/  { "IOC  ", 0, field_decimal },
    /*36*/  { 0, 0, 0 },
    /*37*/  { 0, 0, 0 },
    /*38*/  { 0, 0, 0 },
    /*39*/  { 0, 0, 0 },
    /*40*/  { 0, 0, 0 },
    /*41*/  { 0, opcode_jreg_trans, field_omitted },
    /*42*/  { 0, 0, 0 },
    /*43*/  { 0, 0, 0 },
    /*44*/  { 0, 0, 0 },
    /*45*/  { 0, 0, 0 },
    /*46*/  { 0, 0, 0 },
    /*47*/  { 0, 0, 0 },
    /*48*/  { 0, 0, 0 },
    /*49*/  { 0, opcode_addr_trans, field_omitted },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    /*04*/  { 0, 0, 0 },
    
};

#define is_valid_field(f)   (((f) / 8 <= 5) && ((f) % 8 <= 5))

static char *mix_opcode_decoder(char *buffer, int a, int i, int f, int c) {
    if (c > 63 || c < 0) {
        return NULL;
    }
    
    if (mix_decoding_table[c].opstr != NULL) {
        strncpy(buffer, mix_decoding_table[c].opstr, (size_t)6);
        return buffer;
    }
    
    if (mix_decoding_table[c].opcode == NULL) {
        return NULL;
    }
    
    if ((mix_decoding_table[c].opcode)(buffer, f, c) != NULL) {
        return buffer;
    };    

    return NULL;
}

char *mix_instr_decode(mix_word *w, char *buffer) {
    char instrbuf[6];
    char addrbuf[5];
    char idxbuf[5];
    char fieldbuf[5]; 
    int a = mix_word_value(w, MIX_F(0,2));
    int i = mix_word_value(w, MIX_F(3,3));
    int f = mix_word_value(w, MIX_F(4,4));
    int c = mix_word_value(w, MIX_F(5,5));
    
    if (mix_opcode_decoder(instrbuf, a, i, f, c) == NULL) {
        return NULL;
    } 
    if (a != 0) {
        sprintf(addrbuf, "%d", a);
    } else {
        addrbuf[0] = '\0';
    }
    if (i != 0) {
        sprintf(idxbuf, ",%d", i);
    } else {
        idxbuf[0] = '\0';
    }

    if (mix_decoding_table[c].field != 0) {
        if ((mix_decoding_table[c].field)(fieldbuf, f, c) == NULL) {
            return NULL;
        }
    } else {
        return NULL;
    }

    sprintf(buffer, "%s%s%s%s", instrbuf, addrbuf, idxbuf, fieldbuf);
    return buffer;
}
