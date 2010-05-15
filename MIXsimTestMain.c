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

#include "helloworld.h"

START_TEST (test_framework_exists)
{
	fail_unless(strcmp(getHelloMessage(), "Hello World!\n") == 0, "Test failed!");
}
END_TEST

Suite *hello_suite (void)
{
	Suite *s = suite_create("hello");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_framework_exists);
	suite_add_tcase(s, tc_core);
	
	return s;
}

int main(int argc, char *argv[])
{
	int number_failed;
	Suite *s = hello_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
