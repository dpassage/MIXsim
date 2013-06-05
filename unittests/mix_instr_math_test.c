/*
 *  mix_instr_math_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_math_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_word.h"
#include "mix_instr_math.h"

mix_machine *mix;

static void setup (void);
static void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}

START_TEST(test_DIV_instruction)
{
	mix_word *w = mix_word_create();
    
    int time;
    int ret;
    
    mix_word_set_value(w, MIX_F(0,5), 0);
    mix_machine_load_ra(mix, w);
    
    mix_word_set_value(w, MIX_F(0,5), 17);
    mix_machine_load_rx(mix, w);
    
    mix_word_set_value(w, MIX_F(0,5), 3);
    mix_machine_load_mem(mix, w, 1000);
    
    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    
    ret = mix_machine_instr_DIV(mix, MIX_F(0,5), 1000, 4);
    
    fail_unless(ret == MIX_M_OK, "machine returned error");
    fail_unless(mix_machine_get_time(mix) - time == 12, 
                "Instruction did not take right amount of time");
    fail_unless(mix_machine_get_ip(mix) == 3001, "Instruction did not execute");
    
    mix_machine_read_ra(mix, w);
    fail_unless(mix_word_value(w, MIX_F(0,5)) == 5, "Quotient incorrect");
    mix_machine_read_rx(mix, w);
    fail_unless(mix_word_value(w, MIX_F(0,5)) == 2, "Remainder incorrect");
    fail_unless(mix_machine_get_overflow(mix) == 0, "Overflow incorrect");
}
END_TEST

START_TEST(test_DIV_instruction_FDIV_unimplemented)
{
    int ret;
    
    ret = mix_machine_instr_DIV(mix, 6, 1000, 4);
    
    fail_unless(ret == MIX_M_UNIMPLEMENTED, "should have returned unimplemented");
}
END_TEST


Suite *mix_instr_math_suite(void)
{
	Suite *s = suite_create("mix_instr_math");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_DIV_instruction);
    tcase_add_test (tc_core, test_DIV_instruction_FDIV_unimplemented);
	suite_add_tcase (s, tc_core);
	
	return s;
}
