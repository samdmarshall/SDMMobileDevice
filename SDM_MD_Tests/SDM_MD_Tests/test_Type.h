//
//  test_Type.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_Type_h
#define SDM_MD_Tests_test_Type_h

#include "MobileDevice.h"
#include <SDMMobileDevice/SDMMobileDevice.h>

enum SDM_MD_TestResponse {
    SDM_MD_TestResponse_Invalid = -1,
	SDM_MD_TestResponse_Success,
	SDM_MD_TestResponse_Failure
};

typedef enum SDM_MD_TestResponse SDM_MD_TestResponse;

#define LogTestName printf("%s:\n",__FUNCTION__)

#ifndef DEBUG
#define DEBUG_TEST 0
#else
#define DEBUG_TEST 1
#endif

#define TEST_RESPONSE_STRING(value)  ((value == -1) ? "Invalid" : ((value == 0) ? "Success" : ((value == 1) ? "Failure" : "Error" )))

#define TEST_ASSET(value) printf("\t%s: %s\n",__FUNCTION__,TEST_RESPONSE_STRING(value)); \
if (DEBUG_TEST) { \
assert(value == SDM_MD_TestResponse_Success); \
}


#define TestCount(value) if (value == SDM_MD_TestResponse_Success) { \
test_pass++; \
} \
else { \
test_fail++; \
} \
test_total++;

#endif
