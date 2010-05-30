/*
 *  mixassembleTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixassemble.h"

#include "mixassembleTest.h"

static ma_assembly *ma;

static void setup(void) {
    ma = ma_assembly_create();
}

static void teardown(void) {
}

START_TEST(test_assemble_orig) {
    char *line = "           ORIG 400\n";
    
    int ret = ma_process_line(ma, line);
    fail_unless(ret == 1, "processor should succeed");
    fail_unless(ma_get_current(ma) == 400, "current should be 400");
}
END_TEST

START_TEST(test_assemble_equ) {
    char *line = "FOO        EQU  500\n";
    int value;
    
    int cur = ma_get_current(ma);
    int ret = ma_process_line(ma, line);
    fail_unless(ret == 1, "processor should succeed");
    fail_unless(ma_get_current(ma) == cur, "current should not advance");
    ret = ma_get_symbol(ma, &value, "FOO");
    fail_unless(ret == 1, "value should be set");
    fail_unless(value == 500, "should have set value");
}
END_TEST

START_TEST(test_assemble_comment) {
    char *line = "*HELLO\n";
    
    int cur = ma_get_current(ma);
    int ret = ma_process_line(ma, line);
    fail_unless(ret == 1, "processor should succeed");
    fail_unless(ma_get_current(ma) == cur, "current should not advance");
}
END_TEST

START_TEST(test_translate_opcode) {
    char *opcode = "IN";
    int c;
    int f;
    int ret; 
    
    ret = mixas_encode(opcode, &c, &f);
    fail_unless(ret == 1, "opcode not found!");
    fail_unless(c == 36, "IN should be 36");
    fail_unless(f == -1, "f should be -1, comes from instr");
}
END_TEST

Suite *mixassemble_suite(void)
{
	Suite *s = suite_create("mixassemble");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test(tc_core, test_translate_opcode);
    tcase_add_test(tc_core, test_assemble_orig);
    tcase_add_test(tc_core, test_assemble_equ);
    tcase_add_test(tc_core, test_assemble_comment);
	suite_add_tcase (s, tc_core);
	
	return s;
}