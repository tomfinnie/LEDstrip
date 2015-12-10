// LEDlib implementation
#include "ledlib.h"

// BATTERY LEVEL
// http://www.candlepowerforums.com/vb/showthread.php?81044-Li-Ion-remaining-capacity-vs-voltage
// relies on exact 5v supply for any semblance of accuracy
int calcBatteryPercentage(int batteryRaw)
{
	int batteryPercentage = 0;

	if (batteryRaw > 860) // = 4.2/5 * 1024
	{
		batteryPercentage = 100;
	}
	else if (batteryRaw > 819) // = 4/5 * 1024
	{
		batteryPercentage = 80 + ((batteryRaw - 819) / 2);
	}
	else
	{
		batteryPercentage = batteryRaw - 739;
	}

	return batteryPercentage;
}

// Mode dispatcher
//
// This works by taking a passed-in byte and interpreting that as a number.
// That number is then used as the index into the jump table.  The jump table
// is an array of pointers to functions that embody particular modes (light
// patterns).
//
// We pass in the jump table because this makes it possible for us to mock it
// for testing purposes -- we can supply our own jump table which we can then
// use to see if the correct mode (only) was called.
void dispatchMode(byte mode, FuncPtr jumpTable[])
{
	if(mode<=16)
	{
		jumpTable[mode]();
	}
}
