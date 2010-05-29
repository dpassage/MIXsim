/*
 *  mixparseTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixparseTest.h"
#include "mixtest_lexer_helper.h"

#include "mixlex.h"
#include "mixparse.h"
#include "y.tab.h"

static int commentsfound;
static void test_comment_callback(void) {
    commentsfound++;
}

static void setup(void) {
    mixlex_set_getchar(string_nextchar);
    mixlex_set_ungetchar(string_ungetchar);
    mixparse_reset();
    commentsfound = 0;
}

static void teardown(void) {
}

START_TEST(test_parse_comment)
{
    /* set the lexer to use a string as input */
    char *comment = "* FOO\n";
    string_setup(comment);

    /* set the parser to use the comment testing callback */
    mixparse_setcallback_comment(test_comment_callback);
    
    /* run the parser */
    int ret = mixparse();

    /* ensure the comment callback was called once */
    fail_unless(ret == 0, "parser failed");
    fail_unless(commentsfound == 1, "parser didn't find a comment");
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