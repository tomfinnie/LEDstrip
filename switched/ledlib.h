// LEDlib function declarations

typedef void (*FuncPtr)(void);
typedef unsigned char byte;

int calcBatteryPercentage(int batteryRaw);

void dispatchMode(byte mode, FuncPtr jumpTable[]);