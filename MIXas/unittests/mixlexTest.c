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

static int nextchar(void) {
    if (testcomment[cur] == '\0') {
        return -1;
    }
    
    return (testcomment[cur++]);
}

static void set_test_string(const char *s) {
    testcomment = s;
    cur = 0;
}

START_TEST(test_lex_comment)
{
    set_test_string("* THIS IS A COMMENT\n");
    mixlex_reset();
    mixlex_set_getchar(nextchar);
    int nexttoken = yylex();
    fail_unless(nexttoken == COMMENT, "token should be a comment");
}
END_TEST
    
START_TEST(test_lex_halt)
{
    set_test_string("           HLT\n");
    mixlex_reset();
    mixlex_set_getchar(nextchar);
    int nexttoken = yylex();
    fail_unless(nexttoken == OPCODE, "token should be an opcode");
    fail_unless(strcmp(mixlex_get_token(), "HLT") == 0, "token should be HLT");
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
	tcase_add_test (tc_core, test_lex_comment);
	tcase_add_test (tc_core, test_lex_halt);
	suite_add_tcase (s, tc_core);
	
	return s;
}