/*
 *  mix_instr_load_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_load_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_opcodes.h"
#include "mix_word.h"
#include "mix_instr_load.h"

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

static struct load_reg_case load_largereg_cases[]  = {
	{  5 /* (0:5) */, "- 00 80 03 05 04"},
	{ 13 /* (1:5) */, "+ 00 80 03 05 04"},
	{ 29 /* (3:5) */, "+ 00 00 03 05 04"},
	{  3 /* (0:3) */, "- 00 00 00 80 03"},
	{ 36 /* (4:4) */, "+ 00 00 00 00 05"},
	{  0 /* (0:0) */, "- 00 00 00 00 00"},
	{ -1, NULL}
};


START_TEST(test_LDA_instruction)
{
	mix_word *w = mix_word_create();
	char *testvalue = "- 00 80 03 05 04";
	char *result;
	int time;
	int i;
    int ret;
	
	mix_word_set(w, testvalue);
	mix_machine_load_mem(mix, w, 2000);
    
	for (i = 0; load_largereg_cases[i].f != -1; i++) {        
		time = mix_machine_get_time(mix);
		mix_machine_set_ip(mix, 3000);
        
		ret = mix_machine_instr_LDi(mix, load_largereg_cases[i].f, 2000, MIX_OP_LDA);
        
        fail_unless(ret == MIX_M_OK, "machine returned error");
		
		w = mix_machine_read_ra(mix, w);
		result = mix_word_tostring(w);
		
		fail_unless(mix_machine_get_ip(mix) == 3001, 
                    "Instruction did not execute");
		fail_unless(mix_machine_get_time(mix) - time == 2, 
                    "Instruction did not take right amount of time");
		fail_unless(strcmp(result, load_largereg_cases[i].result) == 0, 
                    "Expected to read %s got %s", load_largereg_cases[i].result, result);
		
	}
}
END_TEST

START_TEST(test_LDX_instruction)
{
	mix_word *w = mix_word_create();
	char *testvalue = "- 00 80 03 05 04";
	char *result;
	int time;
	int i;
    int ret;
	
	mix_word_set(w, testvalue);
	mix_machine_load_mem(mix, w, 2000);
	
	for (i = 0; load_largereg_cases[i].f != -1; i++) {
		
		time = mix_machine_get_time(mix);
		mix_machine_set_ip(mix, 3000);
        
		ret = mix_machine_instr_LDi(mix, load_largereg_cases[i].f, 2000, MIX_OP_LDX);
        
        fail_unless(ret == MIX_M_OK, "machine returned error");
        
		w = mix_machine_read_rx(mix, w);
		result = mix_word_tostring(w);
		
		fail_unless(mix_machine_get_ip(mix) == 3001, 
                    "Instruction did not execute");
		fail_unless(mix_machine_get_time(mix) - time == 2, 
                    "Instruction did not take right amount of time");
		fail_unless(strcmp(result, load_largereg_cases[i].result) == 0, 
                    "Expected to read %s got %s", load_largereg_cases[i].result, result);
		
	}
}
END_TEST

static struct load_reg_case load_smallreg_cases[]  = {
    {  5 /* (0:5) */, "- 00 00 00 05 04"},
    { 13 /* (1:5) */, "+ 00 00 00 05 04"},
    { 29 /* (3:5) */, "+ 00 00 00 05 04"},
    {  3 /* (0:3) */, "- 00 00 00 80 03"},
    { 36 /* (4:4) */, "+ 00 00 00 00 05"},
    {  0 /* (0:0) */, "- 00 00 00 00 00"},
    { -1, NULL }
};

START_TEST(test_LDi_instructions)
{
	mix_word *w = mix_word_create();
	char *testvalue = "- 00 80 03 05 04";
	char *result;
	int time;
    int i;
    int j;
    int ret;
	
	mix_word_set(w, testvalue);
	mix_machine_load_mem(mix, w, 2000);
	
    for (i = 1; i <= 6; i++) {
        for (j = 0; load_smallreg_cases[j].f != -1; j++) {
            
            time = mix_machine_get_time(mix);
            mix_machine_set_ip(mix, 3000);
            
            ret = mix_machine_instr_LDi(mix, load_smallreg_cases[j].f, 
                                        2000, i + MIX_OP_LDA);
            
            fail_unless(ret == MIX_M_OK, "machine returned error");
            
            w = mix_machine_read_ri(mix, w, i);
            result = mix_word_tostring(w);
            
            fail_unless(mix_machine_get_ip(mix) == 3001, 
                        "Instruction did not execute");
            fail_unless(mix_machine_get_time(mix) - time == 2, 
                        "Instruction did not take right amount of time");
            fail_unless(strcmp(result, load_smallreg_cases[j].result) == 0, 
                        "Case %d: Expected to read %s got %s", j, load_smallreg_cases[j].result, result);
            
        }        
    }
}
END_TEST


Suite *mix_instr_load_suite(void)
{
	Suite *s = suite_create("mix_instr_load");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_LDA_instruction);
    tcase_add_test (tc_core, test_LDi_instructions);
    tcase_add_test (tc_core, test_LDX_instruction);
	suite_add_tcase (s, tc_core);
	
	return s;
}