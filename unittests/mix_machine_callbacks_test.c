/*
 *  mix_machine_callbacks_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/26/10.
 *  Copyright 2010 Balveda Software. All rights reserved.
 *
 */

#include "mix_machine.h"
#include "mix_word.h"

#include "mix_machine_callbacks_test.h"

static int callback_ip;
static mix_word callback_word;

static void mix_test_exec_callback(int ip, const mix_word *instr) {
    callback_ip = ip;
    callback_word = *instr;
}

START_TEST(test_exec_callback) {
    mix_machine *mix = mix_machine_create();
    mix_word w;
    char *instr = "+ 00 00 00 18 35";
    int ret;
    
    mix_machine_set_callback_exec(mix, mix_test_exec_callback);
    mark_point();
    mix_word_set(&w, instr);
    mark_point();
    mix_machine_load_mem(mix, &w, 2348);
    mark_point();
    mix_machine_set_ip(mix, 2348);
    mark_point();
    ret = mix_machine_execute(mix);
    mark_point();
            
    fail_unless(ret == MIX_M_OK, "instr failed");
    mark_point();
    fail_unless(callback_ip == 2348, "callback failed to set IP");
    mark_point();
    fail_unless(strcmp(instr, mix_word_tostring(&callback_word)) == 0, "callback failed to set instr");
}
END_TEST

Suite *mix_machine_callbacks_suite(void)
{
	Suite *s = suite_create("mix_machine_callbacks");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_exec_callback);
	suite_add_tcase (s, tc_core);
	
	return s;
}