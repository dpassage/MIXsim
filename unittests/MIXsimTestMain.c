/*
 *  MIXsimTestMain.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/14/10.
 *  Copyright 2010 David Paschich
 *
 */

#include <stdlib.h>
#include <check.h>

#include "mix_device_test.h"
#include "mix_instr_decode_test.h"

#include "mix_instr_addr_xfer_test.h"
#include "mix_instr_comp_test.h"
#include "mix_instr_io_test.h"
#include "mix_instr_jumps_test.h"
#include "mix_instr_load_test.h"
#include "mix_instr_math_test.h"
#include "mix_instr_specials_test.h"
#include "mix_instr_store_test.h"

#include "MIXmachineTest.h"
#include "mix_machine_callbacks_test.h"
#include "MIXwordTest.h"

int main(int argc, char *argv[])
{
	int number_failed;
	
	SRunner *sr = srunner_create(mix_word_suite());
	srunner_add_suite(sr, mix_machine_suite());
	srunner_add_suite(sr, mix_device_suite());

    srunner_add_suite(sr, mix_instr_decode_suite());

    srunner_add_suite(sr, mix_instr_addr_xfer_suite());
    srunner_add_suite(sr, mix_instr_comp_suite());
    srunner_add_suite(sr, mix_instr_io_suite());
    srunner_add_suite(sr, mix_instr_jumps_suite());
    srunner_add_suite(sr, mix_instr_load_suite());
    srunner_add_suite(sr, mix_instr_math_suite());
    srunner_add_suite(sr, mix_instr_specials_suite());
    srunner_add_suite(sr, mix_instr_store_suite());

    srunner_set_fork_status(sr, CK_NOFORK);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
