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

