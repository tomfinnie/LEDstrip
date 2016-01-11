#include "../ledlib.h"
#include "TESTickle.h"
#include <stdlib.h>

void teardown() { }

#define NUM_GLOBALS 10
byte fakeGlobals[NUM_GLOBALS];

void setup() {
	fakeGlobals[0] = NULL;
	fakeGlobals[1] = NULL;
	fakeGlobals[2] = NULL;
	fakeGlobals[3] = NULL;
	fakeGlobals[4] = NULL;
	fakeGlobals[5] = NULL;
	fakeGlobals[6] = NULL;
	fakeGlobals[7] = NULL;
	fakeGlobals[8] = NULL;
	fakeGlobals[9] = NULL;
}

TEST(only_first_mode_called)
	updateGlobals(0,42)
	ASSERT_EQUAL(fakeGlobals[0], 42, "%d", "Expected zeroth global to be 42");
	for (int i = 1; i < NUM_GLOBALS; i++)
	{
		ASSERT_NULL(fakeGlobals[i], "The nth mode has been called");
	}
END_TEST
/*
TEST(over_limit_dispatch_ignored)
	dispatchMode(NUM_GLOBALS, fakeGlobals, NUM_GLOBALS);
	for (int i = 0; i < NUM_GLOBALS; i++)
	{
		ASSERT_NOT_NULL(fakeGlobals[i], "The nth mode has been called");
	}
END_TEST*/
