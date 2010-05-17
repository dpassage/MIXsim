/*
 *  mix_instructions_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instructions_test.h"

/*
 *  MIXmachineTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_word.h"

mix_machine *mix;

void setup (void);
void teardown (void);

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
        
		ret = mix_machine_instr_LDA(mix, load_largereg_cases[i].f, 2000);

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

		ret = mix_machine_instr_LDX(mix, load_largereg_cases[i].f, 2000);

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
	{ -1, NULL}
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
            
            ret = mix_machine_instr_LDi(mix, load_smallreg_cases[j].f, 2000, i);
            
            fail_unless(ret == MIX_M_OK, "machine returned error");
            
            w = mix_machine_read_ri(mix, w, i);
            result = mix_word_tostring(w);
            
            fail_unless(mix_machine_get_ip(mix) == 3001, 
                        "Instruction did not execute");
            fail_unless(mix_machine_get_time(mix) - time == 2, 
                        "Instruction did not take right amount of time");
            fail_unless(strcmp(result, load_smallreg_cases[j].result) == 0, 
                        "Expected to read %s got %s", load_smallreg_cases[j].result, result);
            
        }        
    }
}
END_TEST

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

START_TEST(test_IOC_instruction)
{
	int time;
    int ret;
	mix_device *d; 
    
    d = mix_device_printer_create();
    mix_machine_device_attach(mix, d, 18);
    
    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    
    ret = mix_machine_instr_IOC(mix, 18, 0);

	fail_unless(ret == MIX_M_OK, "machine returned error on execution");
    fail_unless(mix_machine_get_time(mix) - time == 1,  
                "Instruction did not take right amount of time");
    fail_unless(mix_machine_get_ip(mix) == 3001, "IP did not advance");
    fail_unless(mix_device_last_op(d) == MIX_D_P_NEWPAGE, "printer did not get set to new page");
}
END_TEST

START_TEST(test_IOC_unknown_device)
{
	mix_word w;
    int ret;
	mix_device *d; 
    
    d = mix_device_printer_create();
    mix_machine_device_attach(mix, d, 18);
    
    mix_word_set(&w, "+ 00 00 00 12 35"); /* device 12 should be a disk */
    mix_machine_load_mem(mix, &w, 3000);
    
    mix_machine_set_ip(mix, 3000);
    ret = mix_machine_execute(mix);
	fail_unless	(ret == MIX_M_ERROR, "machine should have errored on unknown device");
}
END_TEST

Suite *mix_instructions_suite(void)
{
	Suite *s = suite_create("mix_instructions");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_LDi_instructions);
	tcase_add_test (tc_core, test_LDA_instruction);
	tcase_add_test (tc_core, test_LDX_instruction);
    tcase_add_test (tc_core, test_HLT_instruction);
    tcase_add_test (tc_core, test_IOC_instruction);
    tcase_add_test (tc_core, test_IOC_unknown_device);
	suite_add_tcase (s, tc_core);
	
	return s;
}