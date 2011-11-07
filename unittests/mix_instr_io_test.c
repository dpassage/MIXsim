/*
 *  mix_instr_io_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_instr_io_test.h"

#include "MIXmachineTest.h"

#include "mix_device.h"
#include "mix_machine.h"
#include "mix_opcodes.h"
#include "mix_word.h"
#include "mix_instr_io.h"

mix_machine *mix;

static void setup (void);
static void teardown (void);

void setup (void) {
    mix = mix_machine_create();
}

void teardown (void) {
    mix_machine_destroy(mix);
}


START_TEST(test_IOC_instruction)
{
	int time;
    int ret;
	mix_device *d; 
    
    d = mix_device_printer_create();
    mix_machine_device_attach(mix, d, 18);
    
    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 3000);
    
    ret = mix_machine_instr_IOC(mix, 18, 0, MIX_OP_IOC);
    
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

START_TEST(test_OUT_instruction_printer)
{
    mix_word w;
    int ret;
    int time;
    mix_device *d;

    d = mix_device_printer_create();
    mix_machine_device_attach(mix, d, 18);
    
    for (int i = 3001; i <= 3024; i++) {
        mix_word_set(&w, "+ 00 00 00 00 00");
        mix_machine_load_mem(mix, &w, i);
    }
    
    time = mix_machine_get_time(mix);
    mix_machine_set_ip(mix, 2000);
    
    ret = mix_machine_instr_OUT(mix, 18, 3001, MIX_OP_OUT);

	fail_unless(ret == MIX_M_OK, "machine returned error on execution");
    fail_unless(mix_machine_get_time(mix) - time == 1,  
                "Instruction did not take right amount of time");
    fail_unless(mix_machine_get_ip(mix) == 2001, "IP did not advance");
    fail_unless(mix_device_last_op(d) == MIX_D_P_OUTPUT, "printer did not set output buffer");
}
END_TEST

Suite *mix_instr_io_suite(void)
{
	Suite *s = suite_create("mix_instr_io");
	
	TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test (tc_core, test_IOC_instruction);
    tcase_add_test (tc_core, test_IOC_unknown_device);
    tcase_add_test (tc_core, test_OUT_instruction_printer);
    suite_add_tcase (s, tc_core);
	
	return s;
}