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

static int cur = 0;
static char const *testcomment;
static char ungetbuffer[10];
static int ungetcur = 0;

static int nextchar(void) {
    if (ungetcur > 0) {
        return ungetbuffer[--ungetcur];
    }
    if (testcomment[cur] == '\0') {
        return -1;
    }
    return testcomment[cur++];
}

static int ungetchar(int ch) {
    if (ungetcur == 10) {
        return -1;
    }
    ungetbuffer[ungetcur] = ch;
    ungetcur++;
    return ch;
}

static void set_test_string(const char *s) {
    testcomment = s;
    cur = 0;
}


static void setup(void) {
    mixlex_reset();
    mixlex_set_getchar(nextchar);
    mixlex_set_ungetchar(ungetchar);
}

static void teardown(void) {
}

START_TEST(test_lex_comment)
{
    set_test_string("* THIS IS A COMMENT\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_COMMENT, "token should be a comment");
}
END_TEST
    
START_TEST(test_lex_halt)
{
    set_test_string(" HLT\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_WHITESPACE, "token should be whitespace");
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_SYMBOL, "token should be a symbol");
    fail_unless(strcmp(mixlex_get_token(), "HLT") == 0, "token should be HLT");
    nexttoken = yylex();
    fail_unless(nexttoken == '\n', "token should be newline");
    nexttoken = yylex();
    fail_unless(nexttoken == 0, "we should be done");
}
END_TEST

START_TEST(test_lex_orig)
{
    set_test_string("           ORIG 3000\n");
    int nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_WHITESPACE, "token should be whitespace");
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_SYMBOL, "token should be a symbol");
    fail_unless(strcmp(mixlex_get_token(), "ORIG") == 0, "token should be ORIG");
    nexttoken = yylex();
    fail_unless(nexttoken == MIXAL_WHITESPACE, "token should be whitespace");
    nexttoken = yylex();    
    fail_unless(nexttoken == MIXAL_NUMBER, "token should be a number");
    fail_unless(strcmp(mixlex_get_token(), "3000") == 0, "number should be 3000");
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