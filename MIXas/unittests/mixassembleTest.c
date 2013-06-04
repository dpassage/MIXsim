/*
 *  mixassembleTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mixassemble.h"
#include "mix_word.h"

#include "mixassembleTest.h"

static ma_assembly *ma;

static void setup(void) {
    ma = ma_assembly_create();
}

static void teardown(void) {
}

START_TEST(test_loc_is_garbage) {
    char line[] = "           ORIG 400\n";
    line[0] = -1;

    int ret = ma_process_line(ma, line);
    fail_unless(ret == MA_ERROR, "processor should fail");
}
END_TEST

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
    fail_unless(ret == MA_OK, "processor should succeed");
    fail_unless(ma_get_current(ma) == cur, "current should not advance");
    ret = ma_get_symbol(ma, &value, "FOO");
    fail_unless(ret == MA_OK, "value should be set");
    fail_unless(value == 500, "should have set value");
}
END_TEST

START_TEST(test_assemble_with_label) {
    char *line = "START      HLT\n";
    int value;
    mix_word word;
    
    int cur = ma_get_current(ma);
    int ret = ma_process_line(ma, line);
    
    fail_unless(ret == MA_OK, "processor should succeed");
    fail_unless(ma_get_current(ma) == cur + 1, "current should advance");
    ret = ma_get_symbol(ma, &value, "START");
    fail_unless(ret == MA_OK, "value should have been set");
    fail_unless(value == cur, "value should have been taken from assembly");
    
    ret = ma_get_word(ma, &word, cur);
    fail_unless(ret == MA_OK, "word should have been returned");
    fail_unless(word.bytes[0] == MIX_WORD_PLUS);
    int test_bytes[] = {0, 0, 0, 0, 2, 5};
    for (int i = 1; i <= 5; i++) {
        fail_unless(word.bytes[i] == test_bytes[i], "word %d should have been %d", i, test_bytes[i]);
    }
}
END_TEST
    
START_TEST(test_assemble_comment) {
    char *line = "*HELLO\n";
    
    int cur = ma_get_current(ma);
    int ret = ma_process_line(ma, line);
    fail_unless(ret == MA_OK, "processor should succeed");
    fail_unless(ma_get_current(ma) == cur, "current should not advance");
}
END_TEST

START_TEST(test_translate_opcode) {
    char *opcode = "IN";
    int c;
    int f;
    int ret; 
    
    ret = mixas_encode(opcode, &c, &f);
    fail_unless(ret == MA_OK, "opcode not found!");
    fail_unless(c == 36, "IN should be 36");
    fail_unless(f == -1, "f should be -1, comes from instr");
}
END_TEST

START_TEST(test_end_with_symbol) {
    // set up dummy sumbol
    char *line = "           END  FIN\n";
    char *symbol = "FIN";
    ma_set_symbol(ma, symbol, 1347);

    int ret = ma_process_line(ma, line);
    // assemble/process end line
    // return value should be completed
    fail_unless(ret == MA_DONE, "processor should say assembly done");
    // starting pointer should be equal to value of symbol
    fail_unless(ma_get_start(ma) == 1347);
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
    tcase_add_test(tc_core, test_assemble_with_label);
    tcase_add_test(tc_core, test_loc_is_garbage);
    tcase_add_test(tc_core, test_end_with_symbol);
	suite_add_tcase (s, tc_core);
	
	return s;
}