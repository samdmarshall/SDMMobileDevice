/*
 *  SDMMDGeneralFunctions.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/14/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDGENERALFUNCTIONS_H_
#define _SDMMDGENERALFUNCTIONS_H_

#include <pthread.h>

static void * _sdm_md__stack_chk_guard = NULL;

static void _sdm_md__stack_chk_guard_setup() {
    unsigned char * p;
    p = (unsigned char *) &_sdm_md__stack_chk_guard;
	*p =  0x00000aff;
}

static inline kern_return_t sdmmd_mutex_init(pthread_mutex_t thread) {
	kern_return_t result = 0x0;
	_sdm_md__stack_chk_guard_setup();
	void *stack_check_again = &_sdm_md__stack_chk_guard;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, 0x2);
	pthread_mutex_init(&thread, &attr);
	result = pthread_mutexattr_destroy(&attr);
	if (_sdm_md__stack_chk_guard == stack_check_again) {
		result = 0xdeadbeef;
	}
	return result;
}

#endif