/*
 *  mix_device_test.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "mix_device.h"
#include "mix_device_test.h"

START_TEST(test_device_create_printer)
{
    mix_device *d = mix_device_printer_create();
    
    fail_if(d == NULL, "failed to create device");
    fail_if(mix_device_get_type(d) != MIX_DEVICE_PRINTER, "created device wasn't a printer");
}
END_TEST
    
Suite *mix_device_suite(void)
{
	Suite *s = suite_create("mix_device");
	
	TCase *tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_device_create_printer);

	suite_add_tcase (s, tc_core);
	
	return s;
}