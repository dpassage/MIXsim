/*
 *  mixparse.h
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MIXPARSE_H
#define MIXPARSE_H

void mixparse_reset(void);
void mixparse_setcallback_comment(void (*callback)(void));
int mixparse(void);

void mixparse_comment(void);

#endif