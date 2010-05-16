/*
 *  MIXmachineTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MIXmachineTest.h"

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
	fail_unless(strcmp(expected, s) == 0, "Reading from machine expected %s got %s", expected, s);
}
END_TEST	
	
Suite *mix_machine_suite(void)
{
	Suite *s = suite_create("mix_machine");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_set_memory_cell);
	suite_add_tcase (s, tc_core);
	
	return s;
}