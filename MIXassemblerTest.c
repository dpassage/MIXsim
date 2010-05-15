/*
 *  MIXassemblerTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <check.h>

#include "MIXassemblerTest.h"

#include "mix_word.h"
#include "mix_assembler.h"

START_TEST (test_parse_load)
{
	int result;
	mix_word *w = mix_word_create();
	char *load = "LDA 2000,2(0:3)\n";
	result = mix_assemble_line(load, w);
	fail_unless(result == 0, "Parser failed");
	fail_unless(mix_word_sign(w) == MIX_WORD_PLUS, "Sign of new word is not positive");
	fail_unless(mix_word_byte(w, 1) == 20, "Byte 1 is incorrect");
	fail_unless(mix_word_byte(w, 2) == 00, "Byte 2 is incorrect");
	fail_unless(mix_word_byte(w, 3) == 2, "Byte 3 is incorrect");
	fail_unless(mix_word_byte(w, 4) == 3, "Byte 4 is incorrect");
	fail_unless(mix_word_byte(w, 5) == 8, "Byte 5 is incorrect");
	
}
END_TEST

Suite *mix_assembler_suite(void)
{
	Suite *s = suite_create("mix_assembler");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_parse_load);
	suite_add_tcase (s, tc_core);
	
	return s;
}