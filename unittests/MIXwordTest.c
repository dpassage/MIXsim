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
	int i;
	mix_word *w = mix_word_create();
	fail_unless(w != NULL, "Failed to create mix word");
	fail_unless(mix_word_sign(w) == MIX_WORD_PLUS, 
                "Sign of new word is not positive");
	for (i = 1; i <= 5; i++) {
		fail_unless(mix_word_byte(w, i) == 0, 
                    "Value of byte %d was not zero", i);
	}
}
END_TEST

START_TEST (test_word_setters) 
{
	int i;
	mix_word *w = mix_word_create();
	mix_word_set_sign(w, MIX_WORD_MINUS);
	for (i = 1; i <= 5; i++) {
		mix_word_set_byte(w, i, i);
	}
	
	fail_unless(mix_word_sign(w) == MIX_WORD_MINUS, "Word sign incorrect");
	for (i = 1; i <= 5; i++) {
		fail_unless(mix_word_byte(w, i) == i, 
                    "Value of byte %d was not correct", i);
	}
}
END_TEST

START_TEST (test_word_string_setter)
{
	mix_word *w = mix_word_create();
	char *expected = "- 13 23 18 03 00";
	char *rendered;
	
	mix_word_set(w, expected);
	
	rendered = mix_word_tostring(w);
	
	fail_unless(strcmp(expected, rendered) == 0, 
                "Expected %s but was %s", expected, rendered);
}
END_TEST

START_TEST (test_word_render)
{
	char *expected = "- 13 00 00 03 00";
	mix_word *w = mix_word_create();
	
	mix_word_set_byte(w, 1, 13);
	mix_word_set_byte(w, 4, 3);
	mix_word_set_sign(w, MIX_WORD_MINUS);
	
	char *rendered = mix_word_tostring(w);
	fail_unless(strcmp(rendered, expected) == 0, 
                "Expected %s but was %s", expected, rendered);
}
END_TEST

START_TEST(test_word_clear)
{
	char *expected = "+ 00 00 00 00 00";
	mix_word *w = mix_word_create();
	mix_word_set_byte(w, 1, 13);
	mix_word_set_byte(w, 4, 3);
	mix_word_set_sign(w, MIX_WORD_MINUS);

	mix_word_clear(w);
	
	char *rendered = mix_word_tostring(w);
	fail_unless(strcmp(rendered, expected) == 0, 
                "Expected %s but was %s", expected, rendered);
}
END_TEST

START_TEST(test_word_value)
{
    mix_word *w = mix_word_create();
    mix_word_set(w, "- 13 23 18 03 00");
    
    fail_unless(mix_word_value(w,  2 /* (0:2) */) == -1323);
    fail_unless(mix_word_value(w,  1 /* (0:1) */) ==   -13);
    fail_unless(mix_word_value(w,  9 /* (1:1) */) ==    13);
    fail_unless(mix_word_value(w, 37 /* (4:5) */) ==   300);
    fail_unless(mix_word_value(w, 28 /* (3:4) */) ==  1803);
    fail_unless(mix_word_value(w, 45 /* (5:5) */) ==     0);
}
END_TEST

START_TEST(test_word_set_value)
{
    mix_word *w = mix_word_create();
    mix_word_set(w, "- 13 23 18 03 00");
    
    mix_word_set_value(w, 19 /* (2:3) */, 500);
    fail_unless(strcmp(mix_word_tostring(w), "- 13 05 00 03 00") == 00, "setting 2:3 to 500 didn't work");
}
END_TEST


Suite *mix_word_suite(void)
{
	Suite *s = suite_create("mix_word");

	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_create_word);
	tcase_add_test (tc_core, test_word_setters);
    tcase_add_test (tc_core, test_word_string_setter);
	tcase_add_test (tc_core, test_word_render);	
	tcase_add_test (tc_core, test_word_clear);
    tcase_add_test (tc_core, test_word_value);
    tcase_add_test (tc_core, test_word_set_value);
	suite_add_tcase (s, tc_core);

	return s;
}
