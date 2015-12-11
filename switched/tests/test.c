#include "../ledlib.h"
#include "TESTickle.h"
#include <stdlib.h>


void teardown() { }

//
// Battery Percentage Calculation
//

// FIXME these are not complete!

TEST(upper_limit_over)
    ASSERT_EQUAL(calcBatteryPercentage(861), 100, "%d", "Readings >=861 are 100%");
END_TEST

TEST(upper_limit_at)
    ASSERT_EQUAL(calcBatteryPercentage(860), 100, "%d", "Readings >=861 are 100%");
END_TEST


//
// Mode Dispatch
//

// Function prototypes for our fake operating modes.
// We need these because we need to pop them in our jump table when we define
// it shortly, and the implementations of these functions need to reference
// the jump table (to change it) -- so declaring them (before declaring *how*
// they work) enables us to avoid a Chicken-Egg issue.
void fakeMode0();
void fakeMode1();
void fakeMode2();
void fakeMode3();
void fakeMode4();
void fakeMode5();
void fakeMode6();
void fakeMode7();
void fakeMode8();
void fakeMode9();

#define JUMP_TABLE_SIZE 10
FuncPtr fakeJumpTable[JUMP_TABLE_SIZE];

// Here is our fake jump table
// It's an array of pointers to functions that take no paramaters and do not
// return anything (the same as the real mode functions)
void setup() {
	fakeJumpTable[0] = fakeMode0;
	fakeJumpTable[1] = fakeMode1;
	fakeJumpTable[2] = fakeMode2;
	fakeJumpTable[3] = fakeMode3;
	fakeJumpTable[4] = fakeMode4;
	fakeJumpTable[5] = fakeMode5;
	fakeJumpTable[6] = fakeMode6;
	fakeJumpTable[7] = fakeMode7;
	fakeJumpTable[8] = fakeMode8;
	fakeJumpTable[9] = fakeMode9;

}
// Implementations for the fake mode functions
// These work by setting their corresponding entry in the fake jump table
// to NULL -- giving us a change to the jump table that we can later detect
// to ensure that the correct mode function was called.
// The point is to make these as simple as possible, as this is test code,
// which we don't want to be buggy itself.
// We could've just had these fake mode functions set a global variable to
// indicate which mode was called, but that wouldn't help us detect if more
// than one mode function had (erroneously) been called by the dispatcher.
void fakeMode0()
{
	fakeJumpTable[0] = NULL;
}
void fakeMode1()
{
	fakeJumpTable[1] = NULL;
}
void fakeMode2()
{
	fakeJumpTable[2] = NULL;
}
void fakeMode3()
{
	fakeJumpTable[3] = NULL;
}
void fakeMode4()
{
	fakeJumpTable[4] = NULL;
}
void fakeMode5()
{
	fakeJumpTable[5] = NULL;
}
void fakeMode6()
{
	fakeJumpTable[6] = NULL;
}
void fakeMode7()
{
	fakeJumpTable[7] = NULL;
}
void fakeMode8()
{
	fakeJumpTable[8] = NULL;
}
void fakeMode9()
{
	fakeJumpTable[9] = NULL;
}


// Now we can start testing the dispatchMode function!

TEST(only_first_mode_called)
	dispatchMode(0, fakeJumpTable, JUMP_TABLE_SIZE);
	ASSERT_NULL(fakeJumpTable[0], "The first mode should have been called");
	for (int i=1; i < JUMP_TABLE_SIZE; i++)
	{
		ASSERT_NOT_NULL(fakeJumpTable[i], "The nth mode has been called");
	}
END_TEST

TEST(under_limit_dispatch_ignored)
	dispatchMode(JUMP_TABLE_SIZE, fakeJumpTable, JUMP_TABLE_SIZE);
	for (int i=0; i < JUMP_TABLE_SIZE; i++)
	{
		ASSERT_NOT_NULL(fakeJumpTable[i], "The nth mode has been called");
	}
END_TEST






