#include <FastSPI_LED.h>

#define NUM_LEDS 30

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

#define PIN 4
#define bri 50

int aButton = 32;
int bButton = 33;
int cButton = 34;
int dButton = 35;

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
  
 
//  Serial.begin(9600);
//  pinMode(aButton, INPUT);
//  pinMode(bButton, INPUT);
//  pinMode(cButton, INPUT);
//  pinMode(dButton, INPUT);
}

void loop(){
//  for(int j = 0; j < 6; j++ ){
//    for(int k = 0; k < 255; k++){
//      for(int i = 0; i < NUM_LEDS; i++ ){
//         switch(j) { 
//           case 0: leds[i].g++; break;
//           case 1: leds[i].r--; break;
//           case 2: leds[i].b++; break;
//           case 3: leds[i].g--; break;
//           case 4: leds[i].r++; break;
//           case 5: leds[i].b--; break;
//         }
//      }
//      FastSPI_LED.show();
//      delay(3);
//    }
//  }
//  int target = (digitalRead(dip())*16;

//  leds[0].r=digitalRead(aButton)*(bri+bri*digitalRead(dButton));
//  leds[0].g=digitalRead(bButton)*(bri+bri*digitalRead(dButton));
//  leds[0].b=digitalRead(cButton)*(bri+bri*digitalRead(dButton));

//int speed = (digitalRead(dip())*2;

  int speed = 1;
  int pot0 = A0;
  int pot1 = A1;
  int potValue0 = 0;
  int potValue1 = 0;

// RGB PULSES WITH GAPS
  while( dip() == 0 ){
    speed = analogRead(pot0)/4; 
  
    memset(leds, 0, 3);
  
    for(int j = -1; j < 9; j++ ){
      for(int k = 0; k < (255/speed); k++){
         if(dip() != 0){break;}
         switch(j) { 
           case 3: leds[0].g = leds[0].g + speed; break;
           case 1: leds[0].r = leds[0].r - speed; break;
           case 6: leds[0].b = leds[0].b + speed; break;
           case 4: leds[0].g = leds[0].g - speed; break;
           case 0: leds[0].r = leds[0].r + speed; break;
           case 7: leds[0].b = leds[0].b - speed; break;
           case -1: break;
           case 2: break;
           case 5: break;
           case 8: break;
         }    
          steparray ();
          FastSPI_LED.show();
          delay (analogRead(pot1)/4);
      }
    }
  }
    //delay(25);


//RAINBOWS IN DA HOUSE 

  while(dip() == 1 ){
    speed = analogRead(pot0)/4;
    
    memset(leds, 0, 3);
    leds[0].r = 255;
    
    for(int j = 0; j < 6; j++ ){
      for(int k = 0; k < (255/speed); k++){
        if(dip() != 1){break;}
         switch(j) { 
           case 0: leds[0].g = leds[0].g + speed; break;
           case 1: leds[0].r = leds[0].r - speed; break;
           case 2: leds[0].b = leds[0].b + speed; break;
           case 3: leds[0].g = leds[0].g - speed; break;
           case 4: leds[0].r = leds[0].r + speed; break;
           case 5: leds[0].b = leds[0].b - speed; break;
         }    
         steparray ();
         FastSPI_LED.show();
         delay(analogRead(pot1)/4);
      }
    }
  }
  
//FULLBRITE 

  while( dip() == 2 ){
    potValue0 = analogRead(pot0)/4;
    potValue1 = analogRead(pot1)/4;  
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      leds[i].r = potValue1;
      leds[i].g = potValue0;
      leds[i].b = 255;
    }
  FastSPI_LED.show();
  if(dip() != 2){break;}
  delay(50);
  }
  
//STROBE

  while( dip() == 3 ){
    potValue0 = analogRead(pot0)/4;
    potValue1 = analogRead(pot1)/4;  
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
     // leds[i].r = 255;
     // leds[i].g = 255;
      leds[i].b = 255;
    }
  FastSPI_LED.show();
  if(dip() != 3){break;}
  delay(potValue0);
  memset(leds, 0, NUM_LEDS * 3);
  FastSPI_LED.show();
  delay(potValue1);
  }
  
// USER COLOUR SCROLL

  while( dip() == 4 ){
    leds[0].r = analogRead(pot0)/4;
    leds[0].g = analogRead(pot1)/4;
    steparray ();
    FastSPI_LED.show();
    if(dip() != 4){break;}
    delay(50);
  }
    
//STANDING BY....
  while( dip() > 4 ){
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      memset(leds, 0, NUM_LEDS * 3);
      leds[i].g = 20;
      FastSPI_LED.show();
      if(dip() <= 4){break;}
      delay(50);
    }
  }

    
}

int dip (){
  return (digitalRead(aButton)+digitalRead(bButton)*2+digitalRead(cButton)*4+digitalRead(dButton)*8);
}

void steparray (){
    for(int i = (NUM_LEDS-1); i > 0       ; i--){
    leds[i].r=leds[i-1].r;
    leds[i].g=leds[i-1].g;
    leds[i].b=leds[i-1].b;
    
  }
}


