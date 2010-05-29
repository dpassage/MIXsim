/*
 *  mixparseTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixparseTest.h"

#include "mixlex.h"
#include "y.tab.h"

static void setup(void) {
}

static void teardown(void) {
}

START_TEST(test_parse_comment)
{
    /* set the lexer to use a string as input */
    char *comment = "* FOO";
    
    /* set the parser to use the comment testing callback */
    /* run the parser */
    /* ensure the comment callback was called once */
}
END_TEST

Suite *mixparse_suite(void)
{
	Suite *s = suite_create("mixparse");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test(tc_core, test_parse_comment);
	suite_add_tcase (s, tc_core);
	
	return s;
}