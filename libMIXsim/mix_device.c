/*
 *  mix_device.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "mix_device.h"
#include "mix_word.h"

struct mix_device {
    int last_op;
    int type;
};

void mix_device_control(mix_device *d, int m) {
    printf("Printer: Newpage!\n");
    d->last_op = MIX_D_P_NEWPAGE;
}

int mix_device_last_op(const mix_device *d) {
    return d->last_op;
}

int mix_device_get_type(const mix_device *d) {
    return d->type;
}

mix_device *mix_device_printer_create() {
    mix_device *d = (mix_device *)malloc(sizeof(mix_device));
    d->last_op = 0;
    d->type = MIX_DEVICE_PRINTER;
    return d;
}

static char mix_char_table[] = {
  /*00-07*/  ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
  /*08-15*/  'H', 'I', 't', 'J', 'K', 'L', 'M', 'N',
  /*16-23*/  'O', 'P', 'Q', 'R', 'f', 'p', 'S', 'T',
  /*24-31*/  'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1',
  /*32-39*/  '2', '3', '4', '5', '6', '7', '8', '9',
  /*40-47*/  '.', ',', '(', ')', '+', '-', '*', '/',
  /*48-55*/  '=', '$', '<', '>', '@', ';', ':', '\'',
             '\0'
};

void mix_device_printer_output(mix_device *d, const mix_word *wordbuf) {
    char buffer[122];
    buffer[121] = '\0';
    buffer[120] = '\n';
    int i;
    int w;
    int b;
    char c;
    
    for (i = 119; i >= 0; i--) {
        w = i / 5;
        b = (i % 5) + 1; 
        c = mix_char_table[wordbuf[w].bytes[b]];
        if (c != ' ')
            break;
        buffer[i] = '\0';
    }
    buffer[i+1] = '\n';
    for (; i >= 0; i--) { 
        w = i / 5;
        b = (i % 5) + 1;
        buffer[i] = mix_char_table[wordbuf[w].bytes[b]];
    }
    fputs(buffer, stdout);
    d->last_op = MIX_D_P_OUTPUT;
}
