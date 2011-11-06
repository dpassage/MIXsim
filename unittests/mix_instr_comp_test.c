/*
 *  mix_instr_comp_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_comp_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_opcodes.h"
#include "mix_word.h"
#include "mix_instr_comp.h"

mix_machine *mix;

static void setup (void);
static void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}


START_TEST(test_CMPA_instruction)
{
    mix_word w;
    int ret;
    int time;
    int ip;
    
    mix_word_set_value(&w, MIX_F(0,5), 300);
    mix_machine_load_ra(mix, &w);
    
    mix_word_set_value(&w, MIX_F(0,5), 200);
    mix_machine_load_mem(mix, &w, 3000);
    
    time = mix_machine_get_time(mix);
    ip = mix_machine_get_ip(mix);
    
    ret = mix_machine_instr_CMPA(mix, MIX_F(0,5), 3000, MIX_OP_CMPA);
    
    fail_unless(ret == MIX_M_OK, "CMPA should have executed");
    fail_unless((mix_machine_get_time(mix) - time) == 2, "CMPA should have taken 2 ticks");
    fail_unless((mix_machine_get_ip(mix) - ip) == 1, "CMPA should advances ip");
    fail_unless(mix_machine_get_comparison(mix) == MIX_M_GREATER);
}
END_TEST

Suite *mix_instr_comp_suite(void)
{
	Suite *s = suite_create("mix_instr_comp");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_CMPA_instruction);
    suite_add_tcase (s, tc_core);
	
	return s;
}