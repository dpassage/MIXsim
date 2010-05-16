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

START_TEST(test_create_machine)
{
	mix_machine *m = mix_machine_create();
	
	fail_unless(m != NULL, "Failed to create machine");
}
END_TEST

START_TEST(test_set_memory_cell)
{
	mix_machine *m = mix_machine_create();
	mix_word *w = mix_word_create();
	char *expected = "- 20 00 04 03 02";
	char *s;
	
	mix_word_set(w, expected);
	mix_machine_load_mem(m, w, 3248);
	
	mix_word_destroy(w);
	
	w = mix_word_create();
	mix_machine_read_mem(m, w, 3248);
	s = mix_word_tostring(w);
	fail_unless(strcmp(expected, s) == 0, 
                "Reading from machine expected %s got %s", expected, s);
}
END_TEST

static struct load_reg_case {
	int f;
	char *result;
} load_reg_cases[]  = {
	{  5 /* (0:5) */, "- 00 80 03 05 04"},
	{ 13 /* (1:5) */, "+ 00 80 03 05 04"},
	{ 29 /* (3:5) */, "+ 00 00 03 05 04"},
	{  3 /* (0:3) */, "- 00 00 00 80 03"},
	{ 36 /* (4:4) */, "+ 00 00 00 00 05"},
	{  0 /* (0:0) */, "- 00 00 00 00 00"},
	{ -1, NULL}
};

START_TEST(test_load_register)
{
	mix_word reg;
	mix_word mem;
	char *s;
	
	mix_word_set(&mem, "- 00 80 03 05 04");
	
	for (int i = 0; load_reg_cases[i].f != -1; i++) {
		mix_load_reg(&reg, &mem, load_reg_cases[i].f);
		
		s = mix_word_tostring(&reg);
		fail_unless(strcmp(load_reg_cases[i].result, s) == 0, 
                    "Case %d: Expected %s was %s", 
                    i, load_reg_cases[i].result, s);
		
	}
}
END_TEST

static char *test_LDA[][2] = {
	{ "+ 20 00 00 05 08" /* LDA 2000 */,      "- 00 80 03 05 04"},
	{ "+ 20 00 00 13 08" /* LDA 2000(1:5) */, "+ 00 80 03 05 04"},
	{ NULL, NULL }
};

START_TEST(test_LDA_instruction)
{
	mix_machine *m = mix_machine_create();
	mix_word *w = mix_word_create();
	char *testvalue = "- 00 80 03 05 04";
	char *result;
	int time;
	int i;
    int ret;
	
	mix_word_set(w, testvalue);
	mix_machine_load_mem(m, w, 2000);

	for (i = 0; test_LDA[i][0] != NULL; i++) {
		mix_word_set(w, test_LDA[i][0]);
		mix_machine_load_mem(m, w, 3000);
				
		time = mix_machine_get_time(m);
		mix_machine_set_ip(m, 3000);
		ret = mix_machine_execute(m);
        fail_unless(ret == MIX_M_OK, "machine returned error");
		
		w = mix_machine_read_ra(m, w);
		result = mix_word_tostring(w);
		
		fail_unless(mix_machine_get_ip(m) == 3001, 
                    "Instruction did not execute");
		fail_unless(mix_machine_get_time(m) - time == 2, 
                    "Instruction did not take right amount of time");
		fail_unless(strcmp(result, test_LDA[i][1]) == 0, 
                    "Expected to read %s got %s", test_LDA[i][1], result);
		
	}
}
END_TEST

static char *test_LDX[][2] = {
	{ "+ 20 00 00 05 15" /* LDX 2000 */,      "- 00 80 03 05 04"},
	{ "+ 20 00 00 13 15" /* LDX 2000(1:5) */, "+ 00 80 03 05 04"},
	{ NULL, NULL }
};

START_TEST(test_LDX_instruction)
{
	mix_machine *m = mix_machine_create();
	mix_word *w = mix_word_create();
	char *testvalue = "- 00 80 03 05 04";
	char *result;
	int time;
	int i;
    int ret;
	
	mix_word_set(w, testvalue);
	mix_machine_load_mem(m, w, 2000);
	
	for (i = 0; test_LDX[i][0] != NULL; i++) {
		mix_word_set(w, test_LDX[i][0]);
		mix_machine_load_mem(m, w, 3000);
		
		time = mix_machine_get_time(m);
		mix_machine_set_ip(m, 3000);
		ret = mix_machine_execute(m);
        fail_unless(ret == MIX_M_OK, "machine returned error");

		w = mix_machine_read_rx(m, w);
		result = mix_word_tostring(w);
		
		fail_unless(mix_machine_get_ip(m) == 3001, 
                    "Instruction did not execute");
		fail_unless(mix_machine_get_time(m) - time == 2, 
                    "Instruction did not take right amount of time");
		fail_unless(strcmp(result, test_LDX[i][1]) == 0, 
                    "Expected to read %s got %s", test_LDX[i][1], result);
		
	}
}
END_TEST

START_TEST(test_HLT_instruction)
{
	mix_machine *m = mix_machine_create();
	mix_word w;
	int time;
    int ret;
	
    
    mix_word_set(&w, "+ 00 00 00 02 05");
    mix_machine_load_mem(m, &w, 3000);
    
    time = mix_machine_get_time(m);
    mix_machine_set_ip(m, 3000);
    ret = mix_machine_execute(m);
	fail_if	(ret == MIX_M_ERROR, "machine returned error on execution");
    fail_unless (ret == MIX_M_HALT, "machine should have halted execution");
    fail_unless(mix_machine_get_time(m) - time == 1, 
                "Instruction did not take right amount of time");
}
END_TEST

START_TEST(test_IOC_instruction)
{
	mix_machine *m = mix_machine_create();
	mix_word w;
	int time;
    int ret;
	mix_device *d; 
    
    d = mix_device_printer_create();
    mix_machine_device_attach(m, d, 18);
    
    mix_word_set(&w, "+ 00 00 00 18 35");
    mix_machine_load_mem(m, &w, 3000);
    
    time = mix_machine_get_time(m);
    mix_machine_set_ip(m, 3000);
    ret = mix_machine_execute(m);
	fail_if	(ret == MIX_M_ERROR, "machine returned error on execution");
    fail_unless(mix_machine_get_time(m) - time == 1,  
                "Instruction did not take right amount of time");
    fail_unless(mix_device_last_op(d) == MIX_D_P_NEWPAGE, "printer did not get set to new page");
}
END_TEST

START_TEST(test_IOC_unknown_device)
{
	mix_machine *m = mix_machine_create();
	mix_word w;
    int ret;
	mix_device *d; 
    
    d = mix_device_printer_create();
    mix_machine_device_attach(m, d, 18);
    
    mix_word_set(&w, "+ 00 00 00 12 35"); /* device 12 should be a disk */
    mix_machine_load_mem(m, &w, 3000);
    
    mix_machine_set_ip(m, 3000);
    ret = mix_machine_execute(m);
	fail_unless	(ret == MIX_M_ERROR, "machine should have errored on unknown device");
}
END_TEST

START_TEST(test_device_attach)
{
    mix_machine *m = mix_machine_create();
    mix_device *d = mix_device_printer_create();
    
    mix_machine_device_attach(m, d, 18);
    
    fail_unless(mix_machine_device_get(m, 18) == d, "device attach failed");
}
END_TEST

Suite *mix_machine_suite(void)
{
	Suite *s = suite_create("mix_machine");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_create_machine);
	tcase_add_test (tc_core, test_set_memory_cell);
	tcase_add_test (tc_core, test_LDA_instruction);
	tcase_add_test (tc_core, test_LDX_instruction);
    tcase_add_test (tc_core, test_HLT_instruction);
    tcase_add_test (tc_core, test_IOC_instruction);
    tcase_add_test (tc_core, test_IOC_unknown_device);
	tcase_add_test (tc_core, test_load_register);
    tcase_add_test (tc_core, test_device_attach);
	suite_add_tcase (s, tc_core);
	
	return s;
}