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

static char *test_LDA[][2] = {
	{ "+ 20 00 00 05 08" /* LDA 2000 */,      "- 00 80 03 05 04"},
	{ "+ 20 00 00 13 08" /* LDA 2000(1:5) */, "+ 00 80 03 05 04"},
	{ NULL, NULL }
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
    
	for (i = 0; test_LDA[i][0] != NULL; i++) {
		mix_word_set(w, test_LDA[i][0]);
		mix_machine_load_mem(mix, w, 3000);
        
		time = mix_machine_get_time(mix);
		mix_machine_set_ip(mix, 3000);
		ret = mix_machine_execute(mix);
        fail_unless(ret == MIX_M_OK, "machine returned error");
		
		w = mix_machine_read_ra(mix, w);
		result = mix_word_tostring(w);
		
		fail_unless(mix_machine_get_ip(mix) == 3001, 
                    "Instruction did not execute");
		fail_unless(mix_machine_get_time(mix) - time == 2, 
                    "Instruction did not take right amount of time");
		fail_unless(strcmp(result, test_LDA[i][1]) == 0, 
                    "Expected to read %s got %s", test_LDA[i][1], result);
		
	}
}
END_TEST

static char *test_LDX[][2] = {
	{ "+ 20 00 00 05 15" /* LDX 2000 */,      "- 00 80 03 05 04"},
	{ "+ 20 00 00 13 15" /* LDX 2000(1:5) */, "+ 00 80 03 05 04"},
	{ NULL, NULL }
};

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
	
	for (i = 0; test_LDX[i][0] != NULL; i++) {
		mix_word_set(w, test_LDX[i][0]);
		mix_machine_load_mem(mix, w, 3000);
		
		time = mix_machine_get_time(mix);
		mix_machine_set_ip(mix, 3000);
		ret = mix_machine_execute(mix);
        fail_unless(ret == MIX_M_OK, "machine returned error");
        
		w = mix_machine_read_rx(mix, w);
		result = mix_word_tostring(w);
		
		fail_unless(mix_machine_get_ip(mix) == 3001, 
                    "Instruction did not execute");
		fail_unless(mix_machine_get_time(mix) - time == 2, 
                    "Instruction did not take right amount of time");
		fail_unless(strcmp(result, test_LDX[i][1]) == 0, 
                    "Expected to read %s got %s", test_LDX[i][1], result);
		
	}
}
END_TEST

START_TEST(test_HLT_instruction)
{
	mix_word w;
	int time;
    int ret;
	
    
    mix_word_set(&w, "+ 00 00 00 02 05");
    mix_machine_load_mem(mix, &w, 3000);
    
    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    ret = mix_machine_execute(mix);
	fail_if	(ret == MIX_M_ERROR, "machine returned error on execution");
    fail_unless (ret == MIX_M_HALT, "machine should have halted execution");
    fail_unless(mix_machine_get_time(mix) - time == 1, 
                "Instruction did not take right amount of time");
}
END_TEST

START_TEST(test_IOC_instruction)
{
	mix_word w;
	int time;
    int ret;
	mix_device *d; 
    
    d = mix_device_printer_create();
    mix_machine_device_attach(mix, d, 18);
    
    mix_word_set(&w, "+ 00 00 00 18 35");
    mix_machine_load_mem(mix, &w, 3000);
    
    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    ret = mix_machine_execute(mix);
	fail_if	(ret == MIX_M_ERROR, "machine returned error on execution");
    fail_unless(mix_machine_get_time(mix) - time == 1,  
                "Instruction did not take right amount of time");
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

Suite *mix_instructions_suite(void)
{
	Suite *s = suite_create("mix_instructions");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
	tcase_add_test (tc_core, test_LDA_instruction);
	tcase_add_test (tc_core, test_LDX_instruction);
    tcase_add_test (tc_core, test_HLT_instruction);
    tcase_add_test (tc_core, test_IOC_instruction);
    tcase_add_test (tc_core, test_IOC_unknown_device);
	suite_add_tcase (s, tc_core);
	
	return s;
}