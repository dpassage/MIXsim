/*
 *  mix_instr_addr_xfer_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_addr_xfer_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_opcodes.h"
#include "mix_word.h"
#include "mix_instr_addr_xfer.h"

mix_machine *mix;

static void setup (void);
static void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}


START_TEST(test_INCi_instructions) 
{
    mix_word *w = mix_word_create();
    int ret;
    int time = mix_machine_get_time(mix);
    int ip = mix_machine_get_ip(mix);
    
    /* ensure ri is 0 */
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 0, "register didn't start out at 0");
    
    /* add 9; ensure result is 9 */
    ret = mix_machine_instr_INCi(mix, 0, 9, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    fail_unless(mix_machine_get_time(mix) - time == 1, 
                "instr took wrong amount of time");
    fail_unless(mix_machine_get_ip(mix) - ip == 1, 
                "instr should have advanced ip");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 9, "register should be 9");
    
    /* add 247; ensure result is 256 */
    ret = mix_machine_instr_INCi(mix, 0, 247, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 256, "register should be 9");
    
    /* add -42; ensure result is 214 */
    ret = mix_machine_instr_INCi(mix, 0, -42, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 214, "register should be 9");
    
    /* add 9999; ensure result is MIX_M_UNDEF */
    ret = mix_machine_instr_INCi(mix, 0, 9999, 1);
    fail_unless(ret == MIX_M_UNDEF, "result should be undefined");
}
END_TEST

START_TEST(test_DECi_instructions) 
{
    mix_word *w = mix_word_create();
    int ret;
    int time = mix_machine_get_time(mix);
    int ip = mix_machine_get_ip(mix);
    
    /* set ri to 2000 */
    mix_word_set_value(w, MIX_F(0,5), 2000);
    mix_machine_load_ri(mix, w, 1);
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, MIX_F(0,5)) == 2000, 
                "register didn't start out at 0");
    
    /* subtract 9; ensure result is 19991 */
    ret = mix_machine_instr_DECi(mix, 0, 9, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    fail_unless(mix_machine_get_time(mix) - time == 1, 
                "instr took wrong amount of time");
    fail_unless(mix_machine_get_ip(mix) - ip == 1, 
                "instr should have advanced ip");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 1991, "register should be 1991");
    
    /* subtract 247; ensure result is 1744 */
    ret = mix_machine_instr_DECi(mix, 0, 247, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 1744, "register should be 9");
    
    /* subtract -42; ensure result is 1786 */
    ret = mix_machine_instr_DECi(mix, 0, -42, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 1786, "register should be 9");
    
    /* subtract -9999; ensure result is MIX_M_UNDEF */
    ret = mix_machine_instr_DECi(mix, 0, -9999, 1);
    fail_unless(ret == MIX_M_UNDEF, "result should be undefined");
}
END_TEST

START_TEST(test_ENTi_instructions) 
{
    mix_word *w = mix_word_create();
    int ret;
    int time = mix_machine_get_time(mix);
    int ip = mix_machine_get_ip(mix);
    
    /* ensure ri is 0 */
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, MIX_F(0, 5)) == 0, "register didn't start out at 0");
    
    ret = mix_machine_instr_ENTi(mix, 2, 300, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    fail_unless(mix_machine_get_time(mix) - time == 1, 
                "instr took wrong amount of time");
    fail_unless(mix_machine_get_ip(mix) - ip == 1, 
                "instr should have advanced ip");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 300, "register should be 9");
    
}
END_TEST

START_TEST(test_ENTA_instruction) 
{
	mix_word w;
    int ret;
    int time;
    
    mix_word_set(&w, "+ 00 00 00 02 48"); /* ENTA 0 */
    mix_machine_load_mem(mix, &w, 3000);
    
    mix_word_clear(&w);
    mix_word_set_value(&w, MIX_F(0, 5), 12);
    mix_machine_load_ra(mix, &w);
    
    mix_machine_set_ip(mix, 3000);
    time = mix_machine_get_time(mix);
    
    ret = mix_machine_execute(mix);
	fail_unless	(ret == MIX_M_OK, "ENTA should have executed");
    fail_unless (mix_machine_get_time(mix) - time == 1, "ENTA should have taken one tick");
    fail_unless (mix_machine_get_ip(mix) == 3001, "ENTA should have incremented IP");
    fail_unless (mix_machine_read_ra(mix, &w) == &w, "could not read ra");
    fail_unless (mix_word_value(&w, MIX_F(0, 5)) == 0, "ENTA should be 0");
}
END_TEST


Suite *mix_instr_addr_xfer_suite(void)
{
	Suite *s = suite_create("mix_instr_addr_xfer");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_INCi_instructions);
    tcase_add_test (tc_core, test_DECi_instructions);
    tcase_add_test (tc_core, test_ENTi_instructions);
    tcase_add_test (tc_core, test_ENTA_instruction);

    suite_add_tcase (s, tc_core);
	
	return s;
}
