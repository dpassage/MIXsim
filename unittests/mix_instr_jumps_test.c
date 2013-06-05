/*
 *  mix_instr_jumps_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_jumps_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_opcodes.h"
#include "mix_word.h"
#include "mix_instr_jumps.h"

mix_machine *mix;

static void setup (void);
static void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}


START_TEST(test_JMP_instruction)
{
    int time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    mix_word w;
    
    int ret = mix_machine_instr_JMP(mix, 0, 200);

    fail_unless(ret == MIX_M_OK, "JMP instr failed");
    fail_unless(mix_machine_get_ip(mix) == 200, "JMP failed toset IP");
    /* ensure time elapsed was 1 */
    fail_unless(mix_machine_get_time(mix) - time == 1, "time failed to advance");
    /* ensure reg J is 3001 */
    fail_unless(mix_machine_read_rj(mix, &w) == &w, "failed to read jump register");
    fail_unless(mix_word_value(&w, MIX_F(0, 2)) == 3001, "jump register had wrong value");    
}
END_TEST

START_TEST(test_JG_instruction)
{
    int time;
    int ret;
    
    time = mix_machine_get_time(mix);
    mix_machine_set_comparison(mix, MIX_M_GREATER);
    
    ret = mix_machine_instr_JG(mix, 6, 2137);

    fail_unless(ret == MIX_M_OK);
    fail_unless((mix_machine_get_time(mix) - time) == 1);
    fail_unless(mix_machine_get_ip(mix) == 2137);
}
END_TEST

/* cases[0]: result when reg less than 0 */
/* cases[1]: result when reg equal to 0 */
/* cases[2]: result when reg greater than 0 */
static int jumpreg_tests[][3] = {
    { 1, 0, 0 } , /* N */
    { 0, 1, 0 } , /* Z */
    { 0, 0, 1 } , /* P */
    { 0, 1, 1 } , /* NN */
    { 1, 0, 1 } , /* NZ */
    { 1, 1, 0 } ,  /* NP */
    { -1, -1, -1 }
};

START_TEST(test_Ji_instructions)
{
    int reg;
    int f;
    int testcase;
    
    for (reg = 1; reg <= 6; reg++) {
        for (f = 0; jumpreg_tests[f][0] != -1; f++) {
            for (testcase = 0; testcase < 3; testcase++) {
                int val = testcase - 1;
                mix_word w;
                mix_word_set_value(&w, MIX_F(0, 5), val);
                mix_machine_load_ri(mix, &w, reg);
                int time = mix_machine_get_time(mix);
                mix_machine_set_ip(mix, 3000);
                int ret = mix_machine_instr_Ji(mix, f, 200, MIX_OP_JA + reg);
                
                fail_unless(ret == MIX_M_OK, "Reg %d f %d case %d: Ji instr failed", reg, f, testcase);
                fail_unless(mix_machine_get_time(mix) - time == 1, "Reg %d f %d case %d: time failed to advance", reg, f, testcase);
                fail_unless(mix_machine_read_rj(mix, &w) == &w, "Reg %d f %d case %d: failed to read jump register", reg, f, testcase);
                
                if (jumpreg_tests[f][testcase] == 0) { /* should not jump */
                    fail_unless(mix_machine_get_ip(mix) == 3001, "Reg %d f %d case %d: Ji should not have jumped", reg, f, testcase);
                } else { /* should jump */
                    fail_unless(mix_machine_get_ip(mix) == 200, "Reg %d f %d case %d: JMP failed toset IP", reg, f, testcase);
                    fail_unless(mix_word_value(&w, MIX_F(0, 2)) == 3001, "Reg %d f %d case %d: jump register had wrong value", reg, f, testcase);    
                }
                
            }
        }
    }
}
END_TEST

START_TEST(test_J1Z_instruction) 
{
	mix_word w;
    int ret;
    int time;
    
    mix_word_set(&w, "+ 30 16 00 01 41"); /* J1Z 3016 */
    mix_machine_load_mem(mix, &w, 3000);
    
    mix_word_clear(&w);
    mix_word_set_value(&w, MIX_F(0, 5), 0);
    mix_machine_load_ri(mix, &w, 1);
    
    mix_machine_set_ip(mix, 3000);
    time = mix_machine_get_time(mix);
    
    ret = mix_machine_execute(mix);
	fail_unless	(ret == MIX_M_OK, "J1Z should have executed");
    fail_unless (mix_machine_get_time(mix) - time == 1, "J1Z should have taken one tick");
    fail_unless (mix_machine_get_ip(mix) == 3016, "J1Z should have jumped");
    fail_unless (mix_machine_read_rj(mix, &w) == &w, "could not read rj");
    fail_unless (mix_word_value(&w, MIX_F(0, 2)) == 3001, "J1Z should have stashed IP");
}
END_TEST


Suite *mix_instr_jumps_suite(void)
{
	Suite *s = suite_create("mix_instr_jumps");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_JMP_instruction);
    tcase_add_test (tc_core, test_JG_instruction);
    tcase_add_test (tc_core, test_Ji_instructions);
    tcase_add_test (tc_core, test_J1Z_instruction);
	suite_add_tcase (s, tc_core);
	
	return s;
}
