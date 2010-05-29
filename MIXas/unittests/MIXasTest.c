/*
 *  MIXasTest.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */



#include <stdlib.h>
#include <check.h>

#include "mixlexTest.h"
#include "mixparseTest.h"

int main(int argc, char *argv[])
{
	int number_failed;
	
	SRunner *sr = srunner_create(mixlex_suite());
    srunner_add_suite(sr, mixparse_suite());
    srunner_set_fork_status (sr, CK_NOFORK);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
