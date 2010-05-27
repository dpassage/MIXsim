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
static char *testcomment = "* THIS IS A COMMENT\n";

static int nextchar(void) {
    if (testcomment[cur] == '\0') {
        return -1;
    }
    
    return (testcomment[cur++]);
}
    
START_TEST(test_lexer)
{
    mixlex_set_getchar(nextchar);
    int nexttoken = yylex();
    fail_unless(nexttoken == COMMENT, "token should be a comment");
}
END_TEST
    
Suite *mixlex_suite(void)
{
	Suite *s = suite_create("mixlex");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_lexer);
	suite_add_tcase (s, tc_core);
	
	return s;
}