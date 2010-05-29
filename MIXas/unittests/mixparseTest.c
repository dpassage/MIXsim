/*
 *  mixparseTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixparseTest.h"

#include "mixparse.h"

static int commentsfound;
static void test_comment_callback(void) {
    commentsfound++;
}

static int addressfound;
static int addressvalue;
static void test_address_callback(int i) {
    addressfound++;
    addressvalue = i;
}

static void setup(void) {
    mixparse_reset();
    commentsfound = 0;
    addressfound = 0;
    mixparse_debug();
}

static void teardown(void) {
}

START_TEST(test_parse_comment)
{
    /* set the lexer to use a string as input */
    char *comment = "* FOO\n";
    mixparse_set_input_string(comment);
    
    /* set the parser to use the comment testing callback */
    mixparse_setcallback_comment(test_comment_callback);
    
    /* run the parser */
    int ret = mixparse();

    /* ensure the comment callback was called once */
    fail_unless(ret == 0, "parser failed");
    fail_unless(commentsfound == 1, "parser didn't find a comment");
}
END_TEST

START_TEST(test_parse_address)
{
    /* set the lexer to use a string as input */
    char *orig = "           ORIG 3000\n";
    mixparse_set_input_string(orig);
    
    mixparse_setcallback_address(test_address_callback);
    /* run the parser */
    int ret = mixparse();
    
    /* ensure the comment callback was called once */
    fail_unless(ret == 0, "parser failed");
    fail_unless(addressfound == 1, "parser didn't find an address");
    fail_unless(addressvalue == 3000, "address should have been 3000");
}
END_TEST

START_TEST(test_parse_negnum)
{
    char *equ = "FOO EQU -50\n";
    mixparse_set_input_string(equ);
    
    mixparse_setcallback_address(test_address_callback);
    int ret = mixparse();

    fail_unless(ret == 0, "parser failed");
    fail_unless(addressfound == 1, "parser didn't find an address");
    fail_unless(addressvalue == -50, "address should have been -50");
}
END_TEST

Suite *mixparse_suite(void)
{
	Suite *s = suite_create("mixparse");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test(tc_core, test_parse_comment);
    tcase_add_test(tc_core, test_parse_address);
    tcase_add_test(tc_core, test_parse_negnum);
	suite_add_tcase (s, tc_core);
	
	return s;
}