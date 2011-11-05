/*
 *  mix_op_decoder_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include <check.h>
#include <stdlib.h>

#include "mix_instr_decode_test.h"

#include "mix_instr_decode.h"
#include "mix_word.h"

static char *testcases[][2] = {
    { "+ 00 00 00 00 00", "NOP  "},
	{ "+ 20 00 02 03 08", "LDA  2000,2(0:3)" }, 
    { "+ 00 00 00 02 05", "HLT  "},
    { "+ 30 16 00 01 41", "J1Z  3016"},
    { "+ 00 00 00 02 48", "ENTA 0"},
    { "+ 00 00 02 02 55", "ENTX 0,2" },
    { "+ 20 00 02 11 08", "LDA  2000,2(1:3)" },
    { "- 00 01 03 05 04", "DIV  -1,3" },
    { "+ 30 06 00 01 47", "JXZ  3006" },
    { "- 00 01 03 05 56", "CMPA -1,3" },
	{ NULL, NULL }
};

START_TEST (test_instr_decoder)
{
    mix_word w;
    char *resultbuffer = (char *)malloc(50 * sizeof(char));
    char *result;
    int i;
    
    for (i = 0; testcases[i][0] != NULL; i++) {
        mix_word_set(&w, testcases[i][0]);
        result = mix_instr_decode(&w, resultbuffer);
        fail_unless(result == resultbuffer, "test %d: op decoder should have returned arg buffer", i);
        if (result == NULL) {
            return; /* protect myself from sloppily running tests without forking */
        }
        fail_unless(strcmp(result, testcases[i][1]) == 0, "test %d: Expected %s got %s", i, testcases[i][1], result);
    }
}
END_TEST

Suite *mix_instr_decode_suite(void)
{
	Suite *s = suite_create("mix_op_decoder");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_instr_decoder);
	suite_add_tcase (s, tc_core);
	
	return s;
}