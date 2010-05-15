/*
 *  MIXwordTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/14/10.
 *  Copyright 2010 David Paschich
 *
 */

#include "mix_word.h"
#include "MIXwordTest.h"

START_TEST (test_create_word)
{
	mix_word *w = mix_word_create();
	fail_unless(w != NULL, "Failed to create mix word");
}
END_TEST

Suite *mix_word_suite(void)
{
	Suite *s = suite_create("mix_word");

	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_create_word);
	suite_add_tcase (s, tc_core);

	return s;
}