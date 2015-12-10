// LEDlib
#include <stdio.h>

//
// Typedefs
//

// Here we want a shorthand for saying "this is a pointer to a function that
// takes no paramaters and doesn't return anything".  It is helpful because
// it makes declaring a variable of type "function that takes no parameters
// and doesn't return anything" easy.
//
// E.g. int i -- means "i is an integer"
// E.g. int i[] -- means "i is an array of integers"
//
// We want to make it just as easy to declare variables/structures involving
// pointers to functions that don't take any paramaters and don't return owt.
typedef void (*FuncPtr)(void);

// Everything in C is just meaningless bits, but by calling 'unsigned char' a
// 'byte' it means more to us, and is easier to read and type.
typedef unsigned char byte;

// Note: the above typedefs are *public* -- in the .h file, for anyone using
//       this library to see -- because they are useful in other code too.


//
// Function Prototypes
//

// Based on a raw reading, work out how full the battery is
int calcBatteryPercentage(int batteryRaw);

// Based on a byte we pass in that signifies the mode to use,
// call the appropriate mode function in the jump table.
void dispatchMode(byte mode, FuncPtr jumpTable[], size_t tableSize);
// See the .c file for implementation notes.
// We pass in the jump table so that it can be easily mocked for
// testing purposes.
