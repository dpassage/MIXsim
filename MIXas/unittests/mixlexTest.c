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

#include "mixtest_lexer_helper.h"
#include "mixlexTest.h"

static void setup(void) {
    mixlex_reset();
    mixlex_set_getchar(string_nextchar);
    mixlex_set_ungetchar(string_ungetchar);
}

static void teardown(void) {
}

START_TEST(test_lex_comment)
{
    string_setup("* THIS IS A COMMENT\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_COMMENT, "token should be a comment");
}
END_TEST
    
START_TEST(test_lex_halt)
{
    string_setup(" HLT\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_WHITESPACE, "token should be whitespace");
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_SYMBOL, "token should be a symbol");
    fail_unless(yylval.symbol != NULL, "should have set symbol value");
    if (yylval.symbol != NULL) {
        fail_unless(strcmp(yylval.symbol, "HLT") == 0, "token should be HLT");
    }
    nexttoken = yylex();
    fail_unless(nexttoken == '\n', "token should be newline");
    nexttoken = yylex();
    fail_unless(nexttoken == 0, "we should be done");
}
END_TEST

START_TEST(test_lex_orig)
{
    string_setup("           ORIG 3000\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_WHITESPACE, "token should be whitespace");
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_SYMBOL, "token should be a symbol");
    fail_unless(yylval.symbol != NULL, "should have set symbol value");
    if (yylval.symbol != NULL) {
        fail_unless(strcmp(yylval.symbol, "ORIG") == 0, "token should be ORIG");
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

Suite *mixlex_suite(void)
{
	Suite *s = suite_create("mixlex");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
	tcase_add_test (tc_core, test_lex_comment);
	tcase_add_test (tc_core, test_lex_halt);
	tcase_add_test (tc_core, test_lex_orig);
	suite_add_tcase (s, tc_core);
	
	return s;
}