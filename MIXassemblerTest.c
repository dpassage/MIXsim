/*
 *  MIXassemblerTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <check.h>
#include <stdlib.h>

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

static char *testcases[][2] = {
	{ "LDA 2000,2(0:3)", "+ 20 00 02 03 08" },
	{ "LDA 2000,2(1:3)", "+ 20 00 02 11 08" },
	{ NULL, NULL }
};

START_TEST (test_parser_cases)
{
	int i;
	int parse_code;
	mix_word *w = mix_word_create();
	char *rendered_result;
	
	for (i = 0; testcases[i][0] != NULL; i++) {
		parse_code = mix_assemble_line(testcases[i][0], w);
		fail_unless(parse_code == 0, "Parser failed");
		rendered_result = mix_word_tostring(w);
		fail_unless(strcmp(rendered_result, testcases[i][1]) == 0, "Parser test %d failed: expected %s got %s", i, testcases[i][1], rendered_result);
		free(rendered_result);
	}
}
END_TEST

Suite *mix_assembler_suite(void)
{
	Suite *s = suite_create("mix_assembler");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_parse_load);
	tcase_add_test (tc_core, test_parser_cases);
	suite_add_tcase (s, tc_core);
	
	return s;
}