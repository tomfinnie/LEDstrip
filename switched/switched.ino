#include <FastSPI_LED.h>
#include <math.h>

#define NUM_LEDS 400

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { 
	unsigned char g; 
	unsigned char r; 
	unsigned char b; 
};
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

typedef struct
{
	double R;
	double G;
	double B;
} 
RgbFColor;

typedef void (*FuncPtr)(void);

#define PIN 7
#define bri 50 

#define pot0 A1
#define pot1 A2
#define pot2 A3
#define pot3 A0
#define pot4 A4
#define batteryMonitor A4
#define analogPin A5

#define aButton 4
#define bButton 3
#define cButton 2
#define dButton 6
#define strobePin 8
#define resetPin 9

unsigned long startPhase = 0;
char jumpState = 0;

byte serialDip = 10;
byte speed = 20;
byte groupSize = 5;
float benspeed = 1.3;
byte brightness = 32;
byte globalColour = 0;
byte serialTimeout =0;

int inverseGroupSize = 51;
int red = 0;
int green = 0;
int blue = 127;
int loopHue = 0;
int potValue0 = 0;
int potValue1 = 0;
int potValue2 = 0;
int potValue3 = 0;
int potValue4 = 0;


void setup()
{
	FastSPI_LED.setLeds(NUM_LEDS);
	FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);

	FastSPI_LED.setPin(PIN);

	FastSPI_LED.init();
	FastSPI_LED.start();

	leds = (struct CRGB*)FastSPI_LED.getRGBData();

	pinMode(aButton, INPUT_PULLUP);
	pinMode(bButton, INPUT_PULLUP);
	pinMode(cButton, INPUT_PULLUP);
	pinMode(dButton, INPUT_PULLUP);
	pinMode(5, OUTPUT);
	pinMode(13, OUTPUT);
	digitalWrite(5, LOW);
	digitalWrite(13, LOW);

	pinMode(analogPin, INPUT);
	pinMode(strobePin, OUTPUT);
	pinMode(resetPin, OUTPUT);
	analogReference(DEFAULT);

	digitalWrite(resetPin, LOW);
	digitalWrite(strobePin, HIGH);

	Serial.begin(9600, SERIAL_8N1);
}




// RGB PULSES WITH GAPS
void chaseRGB(){ 

	//blank first LED. 3 for rgb.
	memset(leds, 0, 3);

	//should be rewritten with setHue (easy)

	for(int j = -1; j < 8; j++ ){
		for(int k = 0; k < (groupSize); k++){
			switch(j) {
			case 3: 
				leds[0].g = leds[0].g + inverseGroupSize; 
				break;
			case 1: 
				leds[0].r = leds[0].r - inverseGroupSize; 
				break;
			case 6: 
				leds[0].b = leds[0].b + inverseGroupSize; 
				break;
			case 4: 
				leds[0].g = leds[0].g - inverseGroupSize; 
				break;
			case 0: 
				leds[0].r = leds[0].r + inverseGroupSize; 
				break;
			case 7: 
				leds[0].b = leds[0].b - inverseGroupSize; 
				break;
			case -1: 
				break;
			case 2: 
				break;
			case 5: 
				break;
			}
			steparray ();
			FastSPI_LED.show();
		}
	}
}

//RAINBOWS IN DA HOUSE

//deprecated

void chaseRainbowsOld(){
	memset(leds, 0, 3);
	leds[0].r = 255;

	for(int j = 0; j < 6; j++ ){
		for(int k = 0; k < (groupSize); k++){
			switch(j) {
			case 0: 
				leds[0].g = leds[0].g + inverseGroupSize; 
				break;
			case 1: 
				leds[0].r = leds[0].r - inverseGroupSize; 
				break;
			case 2: 
				leds[0].b = leds[0].b + inverseGroupSize; 
				break;
			case 3: 
				leds[0].g = leds[0].g - inverseGroupSize; 
				break;
			case 4: 
				leds[0].r = leds[0].r + inverseGroupSize; 
				break;
			case 5: 
				leds[0].b = leds[0].b - inverseGroupSize; 
				break;
			}
			steparray ();
		}
	}
}

//FULLBRITE

//Electronics note: this will draw 3A per meter at brightness = 255
//Make sure your PSU / battery is up to this load. You may also have
//problems with voltage drop if you run long strips without suplemental
//power injection.

void chaseFullbrite(){
	potValue0 = analogRead(pot0)/4;
	potValue1 = analogRead(pot1)/4;
	for(int i = 0 ; i < NUM_LEDS; i++ ) {
		leds[i].r = brightness;
		leds[i].g = brightness;
		leds[i].b = brightness;
	}
	FastSPI_LED.show();
	delay(50);
}

//STROBE

//Politics note; this will piss people off, a lot.

void chaseStrobe(){
	setHue (globalColour << 3, brightness);
	for(int i = 1 ; i < NUM_LEDS; i++ ) {
		leds[i].r = leds[0].r ;
		leds[i].g = leds[0].g ;
		leds[i].b = leds[0].b ;
	}
	FastSPI_LED.show();

	delay(speed);
	memset(leds, 0, NUM_LEDS * 3);
	FastSPI_LED.show();
	delay(speed);
}

// USER COLOUR SCROLL

void chaseUserScroll(){
	setHue (globalColour << 3, brightness);
	steparray ();
	FastSPI_LED.show();
}

//BENMODE

void chaseBen() {
	for(int j = 0; j < 3; j++ ){   //head
		leds[0].r = brightness;
		steparray ();
	}

	leds[0].r = brightness;
	leds[0].g = brightness;
	leds[0].b = brightness / benspeed;

	while(leds[0].r > 1){

		leds[0].r = leds[0].r / benspeed; //- potValue1;
		leds[0].g = leds[0].g / benspeed; //- potValue1;
		leds[0].b = leds[0].b / benspeed; //- potValue1/2;

		steparray ();
	}

	memset(leds, 0, 3);

	for(int j = 0; j < (groupSize); j++){
		steparray ();
	}
}

//RANDOMIZER

void chaseRandom(){

	loopHue += random(2*groupSize) - groupSize;
	loopHue = abs(loopHue) % 1536 ;
	setHue (loopHue,255);
	steparray ();
}


//MATRIX

void chaseMatrix(){
	setHue (random(0,1535),brightness);

	while((leds[0].r + leds[0].g +leds[0].b) > 1){
		leds[0].r = leds[0].r / benspeed;
		leds[0].g = leds[0].g / benspeed;
		leds[0].b = leds[0].b / benspeed;
		steparray ();
	}

	memset(leds, 0, 3);

	for(int j = 0; j < (groupSize); j++){
		steparray ();
	}
}

//NEW RAINBOW

void chaseRainbow()
{
	for(int i = 0; i < 1535; i+= inverseGroupSize)
	{
		setHue(i,brightness);
		steparray();
	}
}


//BATTERY LEVEL
//http://www.candlepowerforums.com/vb/showthread.php?81044-Li-Ion-remaining-capacity-vs-voltage
//relies on exact 5v supply for any semblance of accuracy

void chaseBatterylevel()
{
	memset(leds, 0, NUM_LEDS * 3);

	int batteryRaw = 0;
	for (int i = 0; i < 16; i++){
		batteryRaw += analogRead(batteryMonitor);
		delay(10);
	}
	batteryRaw = batteryRaw >> 4;

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

	for(int i = 0 ; i < (NUM_LEDS*batteryPercentage)/100 ; i++ )
	{
		leds[i].r = 50;
	}

	FastSPI_LED.show();
	Serial.print("Battery level is ");
	Serial.println(batteryPercentage);
	delay(100);

}

//FIRE & ICE 1

void chaseFireIce1 ()
{
	setHue(random(0,256),brightness);
	leds[NUM_LEDS/2].r = 0;
	leds[NUM_LEDS/2].g = 0;
	leds[NUM_LEDS/2].b = random (0,brightness);
	steparray();
}

//FIRE & ICE 2


void chaseFireIce2 ()
{
	for(int i = 0; i < 512; i+= inverseGroupSize)
	{
		setHue(abs(256-i),brightness);
		leds[NUM_LEDS/2].r = 0;
		leds[NUM_LEDS/2].g = 0;
		leds[NUM_LEDS/2].b = (i * brightness) >> 9;
		steparray();
	}
}

//STANDING BY....
void chaseStandby ()
{
	for(int i = 0 ; i < NUM_LEDS; i++ ) 
	{
		memset(leds, 0, NUM_LEDS * 3);
		leds[i].g = 255;
		FastSPI_LED.show();
		Serial.print(analogRead(pot0));
		Serial.print("\t");
		Serial.print(analogRead(pot1));
		Serial.print("\t");
		Serial.print(analogRead(pot2));
		Serial.print("\t");
		Serial.print(analogRead(pot3));
		Serial.print("\t");
		Serial.println(analogRead(pot4));

		delay(250);
	}
}

void chaseTemp ()
{
	long temp = readTemp()/10000;
	Serial.println(temp,DEC);
	setTemp((temp-22)*5,(temp-22)*5);
	steparray();
	delay(2000);
}

int dip ()
{
	int readCount = Serial.available();
	byte serialArray[readCount];

	if (readCount == 6) 
	{  
		Serial.readBytes((char *) serialArray,readCount);
		if (serialArray[0] > 0)
		{
			serialDip = serialArray[0];
		}

		if (serialArray[1] > 0)
		{ 
			speed = serialArray[1];
		}

		if (serialArray[2] > 0)
		{
			groupSize = serialArray[2];
			inverseGroupSize = 255/groupSize;
		}

		if (serialArray[3] > 6) //Don't allow very low decay values
		{
			benspeed = 1 + (serialArray[3] >> 7);
		}

		if (serialArray[4] > 0)
		{
			brightness = serialArray[4];
		}

		if (serialArray[5] > 0)
		{
			globalColour = serialArray[5];
		}

		serialTimeout = 0;
	}
	else if (readCount > 6)
	{
		Serial.readBytes((char *) serialArray,readCount);
	}
	else if (serialTimeout > 2)
	{
		Serial.readBytes((char *) serialArray,readCount);
		serialTimeout = 0;
	}
	else
	{
		serialTimeout++;
	}

	serialDebug();
	return (serialDip);
}

//steparray needs much work. The intent is that to save processing at higher speeds
//it will step the array to segments per loop and per fastspi.show. It doesn't work.

void steparray()
{
	int targetDelay = speed;
	int jumps=1;

	for(int i = (NUM_LEDS-1); i > jumps-1 ; i --)
	{
		leds[i].r=leds[i-jumps].r;
		leds[i].g=leds[i-jumps].g;
		leds[i].b=leds[i-jumps].b;
	}

	FastSPI_LED.show();
	if (millis() >= targetDelay+startPhase)
	{
		digitalWrite(13, HIGH);
	}

	while (millis() < targetDelay+startPhase)
	{
		digitalWrite(13, LOW);
		delayMicroseconds(100);
	}

	startPhase = millis();
	jumpState = 0;
}


int doStrobe(){
	digitalWrite(strobePin, LOW);
	delayMicroseconds(100); // to allow the output to settle
	int specLevel = (analogRead(analogPin));
	digitalWrite(strobePin, HIGH);
	delayMicroseconds(100);
	return (specLevel);
}

void setHue(int hue, int localBrightness) {
	unsigned int hueRed = 0;
	unsigned int hueGreen = 0;
	unsigned int hueBlue = 0;

	if (hue < 256){
		hueRed = 255;
		hueGreen = hue;
	}
	else if (hue < 512){
		hueRed = 512-hue;
		hueGreen = 255;
	}
	else if (hue < 768){
		hueGreen = 255;
		hueBlue = hue-512;
	}
	else if (hue < 1024){
		hueGreen = 1024-hue;
		hueBlue = 255;
	}
	else if (hue < 1280){
		hueBlue = 255;
		hueRed = hue-1024;
	}
	else if (hue < 1536){
		hueBlue = 1536-hue;
		hueRed = 255;
	}
	else{
		hueRed = 255;
		hueGreen = 255;
		hueBlue = 255;
	}

	leds[0].r = (hueRed * localBrightness) >> 8;
	leds[0].g = (hueGreen * localBrightness) >> 8;
	leds[0].b = (hueBlue * localBrightness) >> 8;	
}

void serialDebug(){
	/*Serial.print(serialDip,HEX);
	Serial.print("\t");
	Serial.print(speed,HEX);
	Serial.print("\t");
	Serial.print(groupSize,HEX);
	Serial.print("\t");
	Serial.print(benspeed);
	Serial.print("\t");
	Serial.print(brightness,HEX);
	Serial.print("\t");
	Serial.print(globalColour,HEX);
	Serial.print("\t");
	Serial.print("\t");
	Serial.println(Serial.available());*/
}


/*void serialDebug(){
Serial.write(serialDip);
Serial.write(speed); 
Serial.write(groupSize);   
char benspeedchar = ((benspeed - 1) * 128);
Serial.write(benspeedchar);	
Serial.write(brightness);
Serial.write(globalColour);
}*/

//temperature in centigrade * 100
void setTemp(int temperature, int localBrightness){

	if (temperature <= 66)
	{
		red = 255;
	}
	else
	{
		red = temperature - 60;
		red = 329.698727446 * pow(red,-0.133204);
		if (red < 0) { red = 0;}
		if (red > 255) { red = 255;}
	}

	if (temperature <= 66) 
	{
		green = temperature;
		green = 99.4708025861 * log(green) - 161.1195681661;
		if (green < 0 ){ green = 0;}
		if (green > 255) { green = 255;}
	}
	else
	{
		green = temperature - 60;
		green = 288.1221695283 * pow(green,-0.0755148492);
		if (green < 0) { green = 0;}
		if (green > 255) { green = 255;}
	}

	if (temperature >= 66)
	{
		blue = 255;
	}
	else if (temperature <= 19){
		blue = 0;
	}
	else
	{
		blue = temperature - 10;
		blue = 138.5177312231 * log(blue) - 305.0447927307;
		if (blue < 0) { blue = 0;}
		if (blue > 255) { blue = 255;}
	}
	leds[0].r = (red * localBrightness) >> 8;
	leds[0].g = (green * localBrightness) >> 8;
	leds[0].b = (blue * localBrightness) >> 8;	
}

long readTemp() {
	long result;
	// Read temperature sensor against 1.1V reference
	ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);
	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Convert
	while (bit_is_set(ADCSRA,ADSC));
	result = ADCL;
	result |= ADCH<<8;
	result = (result - 125) * 1075;
	return result;
}

FuncPtr jumpTable[] =
{
	chaseStandby,
	chaseRainbowsOld,
	chaseFullbrite,
	chaseStrobe,
	chaseUserScroll,
	chaseBen,
	chaseRandom,
	NULL,
	NULL,
	NULL,
	chaseMatrix,
	chaseRainbow,
	chaseBatterylevel,
	chaseMatrix,
	chaseFireIce1,
	chaseFireIce2,
	chaseTemp
};

void loop()
{
	jumpTable[dip()]();
}