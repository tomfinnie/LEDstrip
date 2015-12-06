#include "../ledlib.h"
#include "TESTickle.h"
#include <stdlib.h>

void setup() { }
void teardown() { }

TEST(upper_limit_over)
    ASSERT_EQUAL(calcBatteryPercentage(861), 100, "%d", "Readings >=861 are 100%");
END_TEST

TEST(upper_limit_at)
    ASSERT_EQUAL(calcBatteryPercentage(860), 100, "%d", "Readings >=861 are 100%");
END_TEST

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
void fakeMode10();
void fakeMode11();
void fakeMode12();
void fakeMode13();
void fakeMode14();
void fakeMode15();
void fakeMode16();

FuncPtr fakeJumpTable[] = {
	fakeMode0,
	fakeMode1,
	fakeMode2,
	fakeMode3,
	fakeMode4,
	fakeMode5,
	fakeMode6,
	fakeMode7,
	fakeMode8,
	fakeMode9,
	fakeMode10,
	fakeMode11,
	fakeMode12,
	fakeMode13,
	fakeMode14,
	fakeMode15,
	fakeMode16
};

void fakeMode0()
{
	fakeJumpTable[0]=NULL;
}
void fakeMode1()
{
	fakeJumpTable[1]=NULL;
}
void fakeMode2()
{
	fakeJumpTable[2]=NULL;
}
void fakeMode3()
{
	fakeJumpTable[3]=NULL;
}
void fakeMode4()
{
	fakeJumpTable[4]=NULL;
}
void fakeMode5()
{
	fakeJumpTable[5]=NULL;
}
void fakeMode6()
{
	fakeJumpTable[6]=NULL;
}
void fakeMode7()
{
	fakeJumpTable[7]=NULL;
}
void fakeMode8()
{
	fakeJumpTable[8]=NULL;
}
void fakeMode9()
{
	fakeJumpTable[9]=NULL;
}
void fakeMode10()
{
	fakeJumpTable[10]=NULL;
}
void fakeMode11()
{
	fakeJumpTable[11]=NULL;
}
void fakeMode12()
{
	fakeJumpTable[12]=NULL;
}
void fakeMode13()
{
	fakeJumpTable[13]=NULL;
}
void fakeMode14()
{
	fakeJumpTable[14]=NULL;
}
void fakeMode15()
{
	fakeJumpTable[15]=NULL;
}
void fakeMode16()
{
	fakeJumpTable[16]=NULL;
}

TEST(first_mode_called)
	dispatchMode(0, fakeJumpTable);
	ASSERT_NULL(fakeJumpTable[0], "It should've called the first one");
END_TEST