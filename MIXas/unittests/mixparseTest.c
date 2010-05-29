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
static int instrfound;
static char const *instrop;
static int instraddress;
static int instrindex;
static int instrfield;

static void test_instruction_callback(const char *op, int a, int i, int f) {
    instrfound++;
    instrop = op;
    instraddress = a;
    instrindex = i;
    instrfield = f;
}
static void test_instruction_reset(void) {
    instrfound = 0;
    instrop = 0;
    instraddress = 0;
    instrindex = 0;
    instrfield = 0;
}    

static void setup(void) {
    mixparse_reset();
    commentsfound = 0;
    addressfound = 0;
    addressvalue = 0;
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
    char *equ = "FOO        EQU -50\n";
    mixparse_set_input_string(equ);
    
    mixparse_setcallback_address(test_address_callback);
    int ret = mixparse();

    fail_unless(ret == 0, "parser failed");
    fail_unless(addressfound == 1, "parser didn't find an address");
    fail_unless(addressvalue == -50, "address should have been -50");
}
END_TEST

START_TEST(test_parse_badcolumns)
{
    char *equ = "FOO   EQU  -50\n";
    mixparse_set_input_string(equ);
    
    int ret = mixparse();
    
    fail_unless(ret == 1, "parser should have failed");
}
END_TEST

static struct instr_test_str {
    char *instr;
    char *op;
    int a;
    int i;
    int f;
} inst_test_str[] = {
    { "           LDA  300\n", "LDA", 300, 0, 0 },
    { "           LDA  3000,4\n", "LDA", 3000, 4, 0 },
    { "           LDA  3000,4(37)\n", "LDA", 3000, 4, 37 },
    { "           LDA  3000,4(0:5)\n", "LDA", 3000, 4, 5 },
    { "           LDA  4:3\n", "LDA", 35, 0, 0 },
    { "           LDA  -1\n", "LDA", -1, 0, 0 },
    { "           LDA  -1+5\n", "LDA", 4, 0, 0 },
    { "           LDA  -1+5*20\n", "LDA", 80, 0, 0 },
    { "           LDA  -1+5*20/6\n", "LDA", 13, 0, 0 },
    { "           LDA  -1-4\n", "LDA", -5, 0, 0 },
    { NULL, NULL, 0, 0, 0 }
};

START_TEST(test_parse_instruction_callback)
{
    mixparse_setcallback_instruction(test_instruction_callback);

    for (int i = 0; inst_test_str[i].instr != NULL; i++) {
        test_instruction_reset();
        mixparse_set_input_string(inst_test_str[i].instr);
    
        int ret = mixparse();
    
        fail_unless(ret == 0, "%d: parser failed", i);
        fail_unless(instrfound == 1, "%d: instruction callback not called", i);
        if (instrop == NULL) {
            fail("%d: instrop wasn't set", i);
        } else {
            fail_unless(strcmp(instrop, inst_test_str[i].op) == 0, "%d: op not set", i);
    }   
        fail_unless(instraddress == inst_test_str[i].a, 
                    "%d: address should be %d was %d", 
                    i, inst_test_str[i].a, instraddress);
        fail_unless(instrindex == inst_test_str[i].i, 
                    "%d: index should be %d was %d", 
                    i, inst_test_str[i].i, instrindex);
        fail_unless(instrfield == inst_test_str[i].f, 
                    "%d: field should be %d was %d", 
                    i, inst_test_str[i].f, instrfield);
    }
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
    tcase_add_test(tc_core, test_parse_badcolumns);
    tcase_add_test(tc_core, test_parse_instruction_callback);
	suite_add_tcase (s, tc_core);
	
	return s;
}