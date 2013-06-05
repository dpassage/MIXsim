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
    mix_word w;
    int i;
	
	fail_unless(m != NULL, "Failed to create machine");
    fail_unless(mix_word_value(mix_machine_read_ra(m, &w), 5 /* (0:5) */) == 0, "ra wasn't empty");
    fail_unless(mix_word_value(mix_machine_read_rx(m, &w), 5 /* (0:5) */) == 0, "rx wasn't empty");
    for (i = 1; i <= 6; i++) {
        fail_unless(mix_word_value(mix_machine_read_ri(m, &w, i), 5 /* (0:5) */) == 0, "r%d wasn't empty", i);
    }
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
	tcase_add_test (tc_core, test_load_register);
    tcase_add_test (tc_core, test_device_attach);
	suite_add_tcase (s, tc_core);
	
	return s;
}
