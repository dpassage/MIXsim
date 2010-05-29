/*
 *  mixlexTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixlex.h"
#include "y.tab.h"

#include "mixlexTest.h"

static void setup(void) {
    mixlex_reset();
}

static void teardown(void) {
}

START_TEST(test_lex_comment)
{
    mixlex_input_string("* THIS IS A COMMENT\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_COMMENT, "token should be a comment");
}
END_TEST
    
START_TEST(test_lex_halt)
{
    mixlex_input_string(" HLT\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_WHITESPACE, "token should be whitespace");
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_SYMBOL, "token should be a symbol");
    fail_unless(yylval.string != NULL, "should have set symbol value");
    if (yylval.string != NULL) {
        fail_unless(strcmp(yylval.string, "HLT") == 0, "token should be HLT");
    }
    nexttoken = yylex();
    fail_unless(nexttoken == '\n', "token should be newline");
    nexttoken = yylex();
    fail_unless(nexttoken == 0, "we should be done");
}
END_TEST

START_TEST(test_lex_orig)
{
    mixlex_input_string("           ORIG 3000\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_WHITESPACE, "token should be whitespace");
    fail_unless(yylval.string != NULL, "should have returned the whitespace");
    if (yylval.string != NULL) {
        fail_unless(strlen(yylval.string) == 11, "should be 11 chars whitespace");
    }
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_SYMBOL, "token should be a symbol");
    fail_unless(yylval.string != NULL, "should have set symbol value");
    if (yylval.string != NULL) {
        fail_unless(strcmp(yylval.string, "ORIG") == 0, "token should be ORIG");
    }
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_WHITESPACE, "token should be whitespace");
    nexttoken = yylex();    
    fail_unless(nexttoken == MIXAL_NUMBER, "token should be a number");
    fail_unless(yylval.val == 3000, "number should be 3000");
    nexttoken = yylex();
    fail_unless(nexttoken == '\n', "token should be newline");
    nexttoken = yylex();
    fail_unless(nexttoken == 0, "we should be done");
}    
END_TEST

START_TEST(test_lex_negative_number) 
{
    mixlex_input_string("-40");
    int nexttoken = yylex();
    fail_unless(nexttoken == '-', "token should be -");
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_NUMBER, "token should be a number");
    fail_unless(yylval.val == 40, "number should be 40");
}
END_TEST

Suite *mixlex_suite(void)
{
	Suite *s = suite_create("mixlex");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
	tcase_add_test (tc_core, test_lex_comment);
	tcase_add_test (tc_core, test_lex_halt);
	tcase_add_test (tc_core, test_lex_orig);
    tcase_add_test (tc_core, test_lex_negative_number);
	suite_add_tcase (s, tc_core);
	
	return s;
}