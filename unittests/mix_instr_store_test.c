/*
 *  mix_instr_store_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_store_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_opcodes.h"
#include "mix_word.h"
#include "mix_instr_store.h"

mix_machine *mix;

static void setup (void);
static void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}

struct load_reg_case {
	int f;
	char *result;
};



static struct load_reg_case store_smallreg_cases[] = {
	{ MIX_F(0, 5) , "+ 00 00 00 09 00"},
	{ MIX_F(1, 5) , "- 00 00 00 09 00"},
	{ MIX_F(5, 5) , "- 01 02 03 04 00"},
	{ MIX_F(2, 2) , "- 01 00 03 04 05"},
	{ MIX_F(2, 3) , "- 01 09 00 04 05"},
	{ MIX_F(0, 1) , "+ 00 02 03 04 05"},
	{ -1, NULL}
};

START_TEST(test_STi_instructions)
{
	mix_word *w = mix_word_create();
    mix_word *loc2000word = mix_word_create();
	char *loc2000val = "- 01 02 03 04 05";
    char *regval = "+ 00 00 00 09 00";
	char *result;
	int time;
    int ret;
	
    mix_word_set(loc2000word, loc2000val);
    
    for (int i = 1; i <= 6; i++) {
        
        mix_word_set(w, regval);
        mix_machine_load_ri(mix, w, i);
        
        for (int j = 0; store_smallreg_cases[j].f != -1; j++) {
            
            mix_machine_load_mem(mix, loc2000word, 2000);
            
            time = mix_machine_get_time(mix);
            mix_machine_set_ip(mix, 3000);
            
            ret = mix_machine_instr_STi(mix, store_smallreg_cases[j].f, 
                                        2000, i + MIX_OP_STA);
            
            fail_unless(ret == MIX_M_OK, "machine returned error");
            
            w = mix_machine_read_mem(mix, w, 2000);
            result = mix_word_tostring(w);
            
            fail_unless(mix_machine_get_ip(mix) == 3001, 
                        "Instruction did not execute");
            fail_unless(mix_machine_get_time(mix) - time == 2, 
                        "Instruction did not take right amount of time");
            fail_unless(strcmp(result, store_smallreg_cases[j].result) == 0, 
                        "Case %d: Expected to read %s got %s", j, store_smallreg_cases[j].result, result);
            
        }        
    }
}
END_TEST


Suite *mix_instr_store_suite(void)
{
	Suite *s = suite_create("mix_instr_store");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_STi_instructions);
	suite_add_tcase (s, tc_core);
	
	return s;
}
