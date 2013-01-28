#include <FastSPI_LED.h>

#define NUM_LEDS 30

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

int serialDip = 12;
int speed = 100;
int groupSize = 5;
int inverseGroupSize = 51;
int red = 0;
int green = 0;
int blue = 127;
float benspeed = 1.3;
int loopHue = 0;
int brightness = 255;

void setup()
{
  FastSPI_LED.setLeds(NUM_LEDS);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_SM16716);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_HL1606);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_595);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);

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


  Serial.begin(9600);
  // pinMode(aButton, INPUT);
  // pinMode(bButton, INPUT);
  // pinMode(cButton, INPUT);
  // pinMode(dButton, INPUT);
}

void loop(){
  int potValue0 = 0;
  int potValue1 = 0;
  int potValue2 = 0;
  int potValue3 = 0;
  int potValue4 = 0;

  // RGB PULSES WITH GAPS
  while( dip() == 10 ){

    memset(leds, 0, 3);

    for(int j = -1; j < 8; j++ ){
      for(int k = 0; k < (groupSize); k++){
        if(dip() != 10){
          break;
        }
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
          //case 8: break;
        }
        steparray ();
        FastSPI_LED.show();
        phaseDelay();
      }
    }
  }
  //delay(25);


  //RAINBOWS IN DA HOUSE

  while(dip() == 1 ){

    //speed =  15;

    memset(leds, 0, 3);
    leds[0].r = 255;

    for(int j = 0; j < 6; j++ ){
	  if(dip() != 1){
         break;
      }
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
        //delay(5);
      }
    }
  }

  //FULLBRITE

  while( dip() == 2 ){
    potValue0 = analogRead(pot0)/4;
    potValue1 = analogRead(pot1)/4;
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      leds[i].r = brightness;
      leds[i].g = brightness;
      leds[i].b = brightness;
    }
    FastSPI_LED.show();
    if(dip() != 2){
      break;
    }
    delay(50);
  }

  //STROBE

  while( dip() == 3 ){
    potValue0 = analogRead(pot0)/4;
    potValue1 = analogRead(pot1)/4;
    potValue2 = analogRead(pot2)/4;
    potValue3 = analogRead(pot3)/4;
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      //fromHSV = RgbF_CreateFromHsv (0,1,1);
      leds[i].r = potValue1 ;
      leds[i].g = potValue2 ;
      leds[i].b = potValue3 ;
      //leds[i] = RgbF_CreateFromHsv (potValue1,1,1);
    }
    FastSPI_LED.show();
    if(dip() != 3){
      break;
    }
    delay(potValue0);
    memset(leds, 0, NUM_LEDS * 3);
    FastSPI_LED.show();
    delay(potValue0);
  }

  // USER COLOUR SCROLL

  while( dip() == 4 ){
    leds[0].r = analogRead(pot1)/4;
    leds[0].g = analogRead(pot2)/4;
    leds[0].b = analogRead(pot3)/4;
    steparray ();
    FastSPI_LED.show();
    if(dip() != 4){
      break;
    }
    phaseDelay();
  }

  //BENMODE

  while( dip() == 5 ){


    //phaseDelay();

    for(int j = 0; j < 3; j++ ){   //head
      leds[0].r = 255;
      //FastSPI_LED.show();
      steparray ();
      if(dip() != 5){
        break;
      }
      //phaseDelay();
    }

    leds[0].r = 255;
    leds[0].g = 255;
    leds[0].b = 255 / benspeed;

    while(leds[0].r > 1){

      leds[0].r = leds[0].r / benspeed; //- potValue1;
      leds[0].g = leds[0].g / benspeed; //- potValue1;
      leds[0].b = leds[0].b / benspeed; //- potValue1/2;

      //FastSPI_LED.show();
      steparray ();
      if(dip() != 5){
        break;
      }
      //phaseDelay();
    }

    memset(leds, 0, 3);

	for(int j = 0; j < (groupSize); j++){
      //FastSPI_LED.show();
      steparray ();
      if(dip() != 5){
        break;
      }
      //phaseDelay();
    }
  }

  //RANDOMIZER

  while( dip() == 6 ){

	loopHue += random(2*groupSize) - groupSize;
	loopHue = abs(loopHue) % 1536 ;
	setHue (loopHue,255);
	steparray ();
  }

  //MSGEQ7 DRIVEN 1

  while( dip() == 7 )
  {

    for (int i = 0; i < 10; i++)
    {
      digitalWrite(resetPin, HIGH);
      digitalWrite(resetPin, LOW);
    }

    leds[0].r = doStrobe()/4;
    doStrobe();
    doStrobe();
    leds[0].g = doStrobe()/4;
    doStrobe();
    doStrobe();
    leds[0].b = doStrobe()/4;

    steparray ();
    if(dip() != 7){
      break;
    }

  }


  //MSGEQ7 DRIVEN 2 

  while( dip() == 8 ){

    memset(leds, 0, NUM_LEDS * 3);

    for (int i = 0; i < 1; i++)
    {
      digitalWrite(resetPin, HIGH);
      digitalWrite(resetPin, LOW);
    }

    int ratio = 1024/(NUM_LEDS+2);
    int spec[3];


    spec[0] = doStrobe()/ratio;
    doStrobe();
    doStrobe();
    spec[1] = doStrobe()/ratio;
    doStrobe();
    doStrobe();
    spec[2] = doStrobe()/ratio;

    for(int j = 0 ; j < 3; j++) 
    {
      for(int i = 0 ; i < spec[j]; i++ ) 
      {
        switch(j) 
        {
        case 0: 
          leds[i].r = 255;
        case 1: 
          leds[i].g = 255;
        case 2: 
          leds[i].b = 255;
        }
      }
    }

    FastSPI_LED.show();

    if(dip() != 8){
      break;
    }

  }

  //MSGEQ7 DRIVEN 3 

  while( dip() == 9 )
  {

    memset(leds, 0, NUM_LEDS * 3);

    for (int i = 0; i < 1; i++)
    {
      digitalWrite(resetPin, HIGH);
      digitalWrite(resetPin, LOW);
    }

    int ratio = 3000/(NUM_LEDS+1);
    int spec[3];
    int offset = NUM_LEDS/3;


    spec[0] = doStrobe()/ratio;
    doStrobe();
    doStrobe();
    spec[1] = doStrobe()/ratio;
    doStrobe();
    doStrobe();
    spec[2] = doStrobe()/ratio;

    for(int j = 0 ; j < 3; j++) 
    {
       
      for(int i = 0 ; i < spec[j]; i++ ) 
      {
        switch(j) 
        {
        case 0: 
          leds[i].r = 127;
        case 1: 
          leds[i+offset].g = 127;
        case 2: 
          leds[i+(offset*2)].b = 127;
        }
      }
    }

    FastSPI_LED.show();

    if(dip() != 9)
    {
      break;
    }

  }

  //MATRIX

  while( dip() == 10 ){
    
	setHue (random(0,1535),255);


    while((leds[0].r + leds[0].g +leds[0].b) > 1){

      leds[0].r = leds[0].r / benspeed;
	  leds[0].g = leds[0].g / benspeed;
	  leds[0].b = leds[0].b / benspeed;

      steparray ();
      if(dip() != 10){
        break;
      }
      //phaseDelay();
    }

    memset(leds, 0, 3);

	for(int j = 0; j < (groupSize); j++){
      //FastSPI_LED.show();
      steparray ();
      if(dip() != 10){
        break;
      }
      //phaseDelay();
    }
  }
	 
  //NEW RAINBOW

  while (dip() == 11)
  {
	  for(int i = 0; i < 1535; i+= inverseGroupSize)
	  {
		  setHue(i,brightness);
		  steparray();
	  }
  }


 //BATTERY LEVEL
//http://www.candlepowerforums.com/vb/showthread.php?81044-Li-Ion-remaining-capacity-vs-voltage

  while (dip() == 12)
  {
	  memset(leds, 0, NUM_LEDS * 3);
	  int batteryRaw = analogRead(batteryMonitor);
	  int batteryPercentage = 0;

	  if (batteryRaw > 860)
	  {batteryPercentage = 100;}

	  else if (batteryRaw > 819)
	  {batteryPercentage = 80 + ((batteryRaw - 819) / 2);}

	  else
	  {batteryPercentage = batteryRaw - 739;}

	  for(int i = 0 ; i < (NUM_LEDS*batteryPercentage)/100 ; i++ )
	  {
		  leds[i].r = 50;
	  }

	  FastSPI_LED.show();
	  Serial.print("Battery level is ");
	  Serial.println(batteryPercentage);
	  delay(1000);

  }




  //STANDING BY....
  while( dip() > 13 )
  {
    for(int i = 0 ; i < NUM_LEDS; i++ ) 
    {
      memset(leds, 0, NUM_LEDS * 3);
      leds[i].g = 255;
      FastSPI_LED.show();
      if(dip() <= 6){
        break;
      }
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
}

int dip ()
{
  //return (digitalRead(aButton)+digitalRead(bButton)*2+digitalRead(cButton)*4+digitalRead(dButton)*8);
  //return(1);
  byte readCount = Serial.available();
  int serialArray[readCount+1];

  if (readCount > 4) 
  {  
    for (int i = 0; i < readCount; i++)
    {
      serialArray[i] = Serial.read();
    }
    if (serialArray[0] > 0)
    {
      serialDip = serialArray[0];
    }
    
    if (serialArray[1] > 0)
    { 
      speed = speed + serialArray[1] - 128;
      if (speed < 1)
      {
        speed = 1;
      }
    }
    
    if (serialArray[2] > 0)
    {
      groupSize = groupSize + serialArray[2] - 128;
      if (groupSize < 1)
      {
        groupSize = 1;
      }
      inverseGroupSize = 255/groupSize;
    }

    if (serialArray[3] > 0)
    {
      benspeed = benspeed + (float(serialArray[3]) - 128)/ 100;
      if (benspeed < 1.05)
      {
        benspeed = 1.05;
      }
    }

	if (serialArray[4] > 0)
	{
		brightness = serialArray[4];
	}

  }
  
  return (serialDip);
}

void steparray()
{
  int targetDelay = speed;
  //int jumps = 1 + int(15/targetDelay);
  int jumps=1;

  //if (jumpState == jumps){
  if(true)
  {

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
    
    //DEBUG REMOVE ME
    
    Serial.print(serialDip);
    Serial.print("\t");
    Serial.print(speed);
    Serial.print("\t");
    Serial.print(groupSize);
    Serial.print("\t");
    Serial.print(inverseGroupSize);
    Serial.print("\t");
    Serial.print(benspeed);
	Serial.print("\t");
    Serial.println(loopHue);
    
    //DEBUG REMOVE ME
    
  } 
  else {
    for(int i = (jumps-1); i > 0 ; i --){
      leds[i].r=leds[i-1].r;
      leds[i].g=leds[i-1].g;
      leds[i].b=leds[i-1].b;
    }
    //jumpState++;
  }
}

void phaseDelay(){}
/*  if (millis() >= (analogRead(pot0)/4+startPhase)){
 digitalWrite(13, HIGH);
 }
 while (millis() < (analogRead(pot0)/4+startPhase)){
 digitalWrite(13, LOW);
 delayMicroseconds(100);
 }
 startPhase = millis();
 }*/

/*int getSpeed(){
  //if (dip()==16){
  if (true){	
    return speed;
  }
  else {
    return ( 1+analogRead(pot0)/4);
  }
}*/

/*int getinverseGroupSize(){
  if (dip()==16){
    return inverseGroupSize;
  }
  else {
    return ( 1+analogRead(pot1)/4);
  }
}*/

int doStrobe(){
  digitalWrite(strobePin, LOW);
  delayMicroseconds(100); // to allow the output to settle
  int specLevel= (analogRead(analogPin));
  digitalWrite(strobePin, HIGH);
  delayMicroseconds(100);
//Serial.print (specLevel);

  return (specLevel);
}

void setHue(int hue, int localBrightness){
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
	
	leds[0].r = (hueRed * localBrightness) >> 8;
	leds[0].g = (hueGreen * localBrightness) >> 8;
	leds[0].b = (hueBlue * localBrightness) >> 8;	
}



