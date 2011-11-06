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


Suite *mix_instr_specials_suite(void)
{
	Suite *s = suite_create("mix_instr_specials");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_HLT_instruction);
	suite_add_tcase (s, tc_core);
	
	return s;
}