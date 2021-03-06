/*
 *  mix_device.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIX_DEVICE_H
#define MIX_DEVICE_H

#include "mix_word.h"

typedef struct mix_device mix_device;

/* general for all devices */

void mix_device_control(mix_device *d, int m);
int mix_device_last_op(const mix_device *d);
int mix_device_get_type(const mix_device *d);
void mix_device_printer_output(mix_device *d, const mix_word *wordbuf);

#define MIX_DEVICE_PRINTER 1
#define MIX_D_P_NEWPAGE 1
#define MIX_D_P_OUTPUT 2

mix_device *mix_device_printer_create(void);

#endif /* MIX_DEVICE_H */
