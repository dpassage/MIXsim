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

#include "MIXwordTest.h"

int main(int argc, char *argv[])
{
	int number_failed;
	Suite *s = mix_word_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
