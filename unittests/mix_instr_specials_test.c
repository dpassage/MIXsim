/*
 *  mix_instr_specials_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_specials_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_word.h"
#include "mix_instr_specials.h"

mix_machine *mix;

static void setup (void);
static void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}

START_TEST(test_HLT_instruction)
{
	int time;
    int ret;
    
    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    
    ret = mix_machine_instr_HLT(mix, 2, 0);
    
    fail_unless (ret == MIX_M_HALT, "machine should have halted execution");
    fail_unless(mix_machine_get_time(mix) - time == 1, 
                "Instruction did not take right amount of time");
}
END_TEST

START_TEST(test_CHAR_instruction)
{
    int time = mix_machine_get_time(mix);
    int ip = mix_machine_get_ip(mix);
    int ret;
    mix_word w;
    
    mix_word_set_value(&w, MIX_F(0,5), 1234567890);
    mix_machine_load_ra(mix, &w);
    
    ret = mix_machine_instr_CHAR(mix, 2, 0);
    
    fail_unless(ret == MIX_M_OK, "instr should have executed");
    fail_unless(mix_machine_get_time(mix) - time == 1, "instr should take 1 tick");
    fail_unless(mix_machine_get_ip(mix) - ip == 1, "ip should advance");
    
    mix_machine_read_ra(mix, &w);
    fail_unless(w.bytes[1] == 31);
    fail_unless(w.bytes[2] == 32);
    fail_unless(w.bytes[3] == 33);
    fail_unless(w.bytes[4] == 34);
    fail_unless(w.bytes[5] == 35);

    mix_machine_read_rx(mix, &w);
    fail_unless(w.bytes[1] == 36);
    fail_unless(w.bytes[2] == 37);
    fail_unless(w.bytes[3] == 38);
    fail_unless(w.bytes[4] == 39);
    fail_unless(w.bytes[5] == 30);
    
}
END_TEST

Suite *mix_instr_specials_suite(void)
{
	Suite *s = suite_create("mix_instr_specials");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_HLT_instruction);
    tcase_add_test (tc_core, test_CHAR_instruction);
	suite_add_tcase (s, tc_core);
	
	return s;
}
