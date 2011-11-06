/*
 *  mix_instr_jumps_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_jumps_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_word.h"
#include "mix_instr_jumps.h"

mix_machine *mix;

static void setup (void);
static void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}


START_TEST(test_JMP_instruction)
{
    int time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    mix_word w;
    
    int ret = mix_machine_instr_JMP(mix, 0, 200);

    fail_unless(ret == MIX_M_OK, "JMP instr failed");
    fail_unless(mix_machine_get_ip(mix) == 200, "JMP failed toset IP");
    /* ensure time elapsed was 1 */
    fail_unless(mix_machine_get_time(mix) - time == 1, "time failed to advance");
    /* ensure reg J is 3001 */
    fail_unless(mix_machine_read_rj(mix, &w) == &w, "failed to read jump register");
    fail_unless(mix_word_value(&w, MIX_F(0, 2)) == 3001, "jump register had wrong value");    
}
END_TEST

START_TEST(test_JG_instruction)
{
    int time;
    int ret;
    
    time = mix_machine_get_time(mix);
    mix_machine_set_comparison(mix, MIX_M_GREATER);
    
    ret = mix_machine_instr_JG(mix, 6, 2137);

    fail_unless(ret == MIX_M_OK);
    fail_unless((mix_machine_get_time(mix) - time) == 1);
    fail_unless(mix_machine_get_ip(mix) == 2137);
}
END_TEST

Suite *mix_instr_jumps_suite(void)
{
	Suite *s = suite_create("mix_instr_jumps");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_JMP_instruction);
    tcase_add_test (tc_core, test_JG_instruction);
	suite_add_tcase (s, tc_core);
	
	return s;
}