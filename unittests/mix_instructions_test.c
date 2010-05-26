/*
 *  mix_instructions_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instructions_test.h"

/*
 *  MIXmachineTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_word.h"

mix_machine *mix;

void setup (void);
void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}

struct load_reg_case {
	int f;
	char *result;
};

static struct load_reg_case load_largereg_cases[]  = {
	{  5 /* (0:5) */, "- 00 80 03 05 04"},
	{ 13 /* (1:5) */, "+ 00 80 03 05 04"},
	{ 29 /* (3:5) */, "+ 00 00 03 05 04"},
	{  3 /* (0:3) */, "- 00 00 00 80 03"},
	{ 36 /* (4:4) */, "+ 00 00 00 00 05"},
	{  0 /* (0:0) */, "- 00 00 00 00 00"},
	{ -1, NULL}
};

START_TEST(test_LDA_instruction)
{
	mix_word *w = mix_word_create();
	char *testvalue = "- 00 80 03 05 04";
	char *result;
	int time;
	int i;
    int ret;
	
	mix_word_set(w, testvalue);
	mix_machine_load_mem(mix, w, 2000);
    
	for (i = 0; load_largereg_cases[i].f != -1; i++) {        
		time = mix_machine_get_time(mix);
		mix_machine_set_ip(mix, 3000);
        
		ret = mix_machine_instr_LDA(mix, load_largereg_cases[i].f, 2000);

        fail_unless(ret == MIX_M_OK, "machine returned error");
		
		w = mix_machine_read_ra(mix, w);
		result = mix_word_tostring(w);
		
		fail_unless(mix_machine_get_ip(mix) == 3001, 
                    "Instruction did not execute");
		fail_unless(mix_machine_get_time(mix) - time == 2, 
                    "Instruction did not take right amount of time");
		fail_unless(strcmp(result, load_largereg_cases[i].result) == 0, 
                    "Expected to read %s got %s", load_largereg_cases[i].result, result);
		
	}
}
END_TEST

START_TEST(test_LDX_instruction)
{
	mix_word *w = mix_word_create();
	char *testvalue = "- 00 80 03 05 04";
	char *result;
	int time;
	int i;
    int ret;
	
	mix_word_set(w, testvalue);
	mix_machine_load_mem(mix, w, 2000);
	
	for (i = 0; load_largereg_cases[i].f != -1; i++) {
		
		time = mix_machine_get_time(mix);
		mix_machine_set_ip(mix, 3000);

		ret = mix_machine_instr_LDX(mix, load_largereg_cases[i].f, 2000);

        fail_unless(ret == MIX_M_OK, "machine returned error");
        
		w = mix_machine_read_rx(mix, w);
		result = mix_word_tostring(w);
		
		fail_unless(mix_machine_get_ip(mix) == 3001, 
                    "Instruction did not execute");
		fail_unless(mix_machine_get_time(mix) - time == 2, 
                    "Instruction did not take right amount of time");
		fail_unless(strcmp(result, load_largereg_cases[i].result) == 0, 
                    "Expected to read %s got %s", load_largereg_cases[i].result, result);
		
	}
}
END_TEST

static struct load_reg_case load_smallreg_cases[]  = {
    {  5 /* (0:5) */, "- 00 00 00 05 04"},
    { 13 /* (1:5) */, "+ 00 00 00 05 04"},
    { 29 /* (3:5) */, "+ 00 00 00 05 04"},
    {  3 /* (0:3) */, "- 00 00 00 80 03"},
    { 36 /* (4:4) */, "+ 00 00 00 00 05"},
    {  0 /* (0:0) */, "- 00 00 00 00 00"},
    { -1, NULL }
};

START_TEST(test_LDi_instructions)
{
	mix_word *w = mix_word_create();
	char *testvalue = "- 00 80 03 05 04";
	char *result;
	int time;
    int i;
    int j;
    int ret;
	
	mix_word_set(w, testvalue);
	mix_machine_load_mem(mix, w, 2000);
	
    for (i = 1; i <= 6; i++) {
        for (j = 0; load_smallreg_cases[j].f != -1; j++) {
            
            time = mix_machine_get_time(mix);
            mix_machine_set_ip(mix, 3000);
            
            ret = mix_machine_instr_LDi(mix, load_smallreg_cases[j].f, 2000, i);
            
            fail_unless(ret == MIX_M_OK, "machine returned error");
            
            w = mix_machine_read_ri(mix, w, i);
            result = mix_word_tostring(w);
            
            fail_unless(mix_machine_get_ip(mix) == 3001, 
                        "Instruction did not execute");
            fail_unless(mix_machine_get_time(mix) - time == 2, 
                        "Instruction did not take right amount of time");
            fail_unless(strcmp(result, load_smallreg_cases[j].result) == 0, 
                        "Case %d: Expected to read %s got %s", j, load_smallreg_cases[j].result, result);
            
        }        
    }
}
END_TEST

static struct load_reg_case store_smallreg_cases[] = {
	{ MIX_F(0, 5) , "+ 00 00 00 09 00"},
	{ MIX_F(1, 5) , "- 00 00 00 09 00"},
	{ MIX_F(5, 5) , "- 01 02 03 04 00"},
	{ MIX_F(2, 2) , "- 01 00 03 04 05"},
	{ MIX_F(2, 3) , "- 01 09 00 04 05"},
	{ MIX_F(0, 1) , "+ 00 02 03 04 05"},
	{ -1, NULL}
};

START_TEST(test_STi_instructions)
{
	mix_word *w = mix_word_create();
    mix_word *loc2000word = mix_word_create();
	char *loc2000val = "- 01 02 03 04 05";
    char *regval = "+ 00 00 00 09 00";
	char *result;
	int time;
    int ret;
	
    mix_word_set(loc2000word, loc2000val);
    
    for (int i = 1; i <= 6; i++) {
        
        mix_word_set(w, regval);
        mix_machine_load_ri(mix, w, i);
        
        for (int j = 0; load_smallreg_cases[j].f != -1; j++) {
            
            mix_machine_load_mem(mix, loc2000word, 2000);
            
            time = mix_machine_get_time(mix);
            mix_machine_set_ip(mix, 3000);
            
            ret = mix_machine_instr_STi(mix, store_smallreg_cases[j].f, 2000, i);
            
            fail_unless(ret == MIX_M_OK, "machine returned error");
            
            w = mix_machine_read_mem(mix, w, 2000);
            result = mix_word_tostring(w);
            
            fail_unless(mix_machine_get_ip(mix) == 3001, 
                        "Instruction did not execute");
            fail_unless(mix_machine_get_time(mix) - time == 2, 
                        "Instruction did not take right amount of time");
            fail_unless(strcmp(result, store_smallreg_cases[j].result) == 0, 
                        "Case %d: Expected to read %s got %s", j, load_smallreg_cases[j].result, result);
            
        }        
    }
}
END_TEST

START_TEST(test_HLT_instruction)
{
	int time;
    int ret;

    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    
    ret = mix_machine_instr_HLT(mix, 2, 0);

    fail_unless (ret == MIX_M_HALT, "machine should have halted execution");
    fail_unless(mix_machine_get_time(mix) - time == 1, 
                "Instruction did not take right amount of time");
}
END_TEST

START_TEST(test_IOC_instruction)
{
	int time;
    int ret;
	mix_device *d; 
    
    d = mix_device_printer_create();
    mix_machine_device_attach(mix, d, 18);
    
    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    
    ret = mix_machine_instr_IOC(mix, 18, 0);

	fail_unless(ret == MIX_M_OK, "machine returned error on execution");
    fail_unless(mix_machine_get_time(mix) - time == 1,  
                "Instruction did not take right amount of time");
    fail_unless(mix_machine_get_ip(mix) == 3001, "IP did not advance");
    fail_unless(mix_device_last_op(d) == MIX_D_P_NEWPAGE, "printer did not get set to new page");
}
END_TEST

START_TEST(test_IOC_unknown_device)
{
	mix_word w;
    int ret;
	mix_device *d; 
    
    d = mix_device_printer_create();
    mix_machine_device_attach(mix, d, 18);
    
    mix_word_set(&w, "+ 00 00 00 12 35"); /* device 12 should be a disk */
    mix_machine_load_mem(mix, &w, 3000);
    
    mix_machine_set_ip(mix, 3000);
    ret = mix_machine_execute(mix);
	fail_unless	(ret == MIX_M_ERROR, "machine should have errored on unknown device");
}
END_TEST

START_TEST(test_INCi_instructions) 
{
    mix_word *w = mix_word_create();
    int ret;
    int time = mix_machine_get_time(mix);
    int ip = mix_machine_get_ip(mix);
    
    /* ensure ri is 0 */
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 0, "register didn't start out at 0");
    
    /* add 9; ensure result is 9 */
    ret = mix_machine_instr_INCi(mix, 0, 9, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    fail_unless(mix_machine_get_time(mix) - time == 1, 
                "instr took wrong amount of time");
    fail_unless(mix_machine_get_ip(mix) - ip == 1, 
                "instr should have advanced ip");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 9, "register should be 9");
    
    /* add 247; ensure result is 256 */
    ret = mix_machine_instr_INCi(mix, 0, 247, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 256, "register should be 9");

    /* add -42; ensure result is 214 */
    ret = mix_machine_instr_INCi(mix, 0, -42, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 214, "register should be 9");

    /* add 9999; ensure result is MIX_M_UNDEF */
    ret = mix_machine_instr_INCi(mix, 0, 9999, 1);
    fail_unless(ret == MIX_M_UNDEF, "result should be undefined");
}
END_TEST

START_TEST(test_ENTi_instructions) 
{
    mix_word *w = mix_word_create();
    int ret;
    int time = mix_machine_get_time(mix);
    int ip = mix_machine_get_ip(mix);
    
    /* ensure ri is 0 */
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, MIX_F(0, 5)) == 0, "register didn't start out at 0");
 
    ret = mix_machine_instr_ENTi(mix, 2, 300, 1);
    fail_unless(ret == MIX_M_OK, "instr should have succeeded");
    fail_unless(mix_machine_get_time(mix) - time == 1, 
                "instr took wrong amount of time");
    fail_unless(mix_machine_get_ip(mix) - ip == 1, 
                "instr should have advanced ip");
    w = mix_machine_read_ri(mix, w, 1);
    fail_unless(mix_word_value(w, 5) == 300, "register should be 9");
    
}
END_TEST

START_TEST(test_ENTA_instruction) 
{
	mix_word w;
    int ret;
    int time;
    
    mix_word_set(&w, "+ 00 00 00 02 48"); /* ENTA 0 */
    mix_machine_load_mem(mix, &w, 3000);
    
    mix_word_clear(&w);
    mix_word_set_value(&w, MIX_F(0, 5), 12);
    mix_machine_load_ra(mix, &w);
    
    mix_machine_set_ip(mix, 3000);
    time = mix_machine_get_time(mix);
    
    ret = mix_machine_execute(mix);
	fail_unless	(ret == MIX_M_OK, "ENTA should have executed");
    fail_unless (mix_machine_get_time(mix) - time == 1, "ENTA should have taken one tick");
    fail_unless (mix_machine_get_ip(mix) == 3001, "ENTA should have incremented IP");
    fail_unless (mix_machine_read_ra(mix, &w) == &w, "could not read ra");
    fail_unless (mix_word_value(&w, MIX_F(0, 5)) == 0, "ENTA should be 0");
}
END_TEST


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
                int ret = mix_machine_instr_Ji(mix, f, 200, reg);

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

Suite *mix_instructions_suite(void)
{
	Suite *s = suite_create("mix_instructions");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_LDi_instructions);
	tcase_add_test (tc_core, test_LDA_instruction);
	tcase_add_test (tc_core, test_LDX_instruction);
    tcase_add_test (tc_core, test_STi_instructions);
    tcase_add_test (tc_core, test_HLT_instruction);
    tcase_add_test (tc_core, test_IOC_instruction);
    tcase_add_test (tc_core, test_IOC_unknown_device);
    tcase_add_test (tc_core, test_INCi_instructions);
    tcase_add_test (tc_core, test_JMP_instruction);
    tcase_add_test (tc_core, test_Ji_instructions);
    tcase_add_test (tc_core, test_J1Z_instruction);
    tcase_add_test (tc_core, test_ENTi_instructions);
    tcase_add_test (tc_core, test_ENTA_instruction);
	suite_add_tcase (s, tc_core);
	
	return s;
}