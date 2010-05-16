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
};

void mix_device_control(mix_device *d, int m) {
    printf("Printer: Newpage!\n");
    d->last_op = MIX_D_P_NEWPAGE;
}

int mix_device_last_op(mix_device *d) {
    return d->last_op;
}

mix_device *mix_device_printer_create() {
    return (mix_device *)malloc(sizeof(mix_device));
}

