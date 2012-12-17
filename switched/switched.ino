#include <FastSPI_LED.h>

#define NUM_LEDS 268

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

typedef struct
{
    double R;
    double G;
    double B;
} RgbFColor;

#define PIN 7
#define bri 50

int aButton = 4;
int bButton = 3;
int cButton = 2;
int dButton = 1;


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
  digitalWrite(5, LOW);
  
 
//  Serial.begin(9600);
// pinMode(aButton, INPUT);
// pinMode(bButton, INPUT);
// pinMode(cButton, INPUT);
// pinMode(dButton, INPUT);
}

void loop(){
  int speed = 1;
  int pot0 = A1;
  int pot1 = A2;
  int pot2 = A3;
  int pot3 = A0;
  int pot4 = A4;

  int potValue0 = 0;
  int potValue1 = 0;
  int potValue2 = 0;
  int potValue3 = 0;
  int potValue4 = 0;
  float benspeed = 0;

// RGB PULSES WITH GAPS
  while( dip() == 0 ){
    speed = analogRead(pot0)/4;
  
    memset(leds, 0, 3);
  
    for(int j = -1; j < 8; j++ ){
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
           //case 8: break;
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
    //speed =  15;
    
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
         //delay(5);
      }
    }
  }
  
//FULLBRITE

  while( dip() == 2 ){
    potValue0 = analogRead(pot0)/4;
    potValue1 = analogRead(pot1)/4;
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      leds[i].r = potValue0;
      leds[i].g = potValue0;
      leds[i].b = potValue0;
    }
  FastSPI_LED.show();
  if(dip() != 2){break;}
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
  if(dip() != 3){break;}
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
    if(dip() != 4){break;}
    delay(analogRead(pot0)/4);
  }

//BENMODE

  while( dip() == 5 ){
    
    
    
    potValue0 = analogRead(pot0)/4;
    potValue1 = (analogRead(pot1)/4)+1;
    benspeed = 1024 + analogRead(pot3);
    benspeed /= 1024;
    
    for(int j = 0; j < 3; j++ ){   //head
      leds[0].r = 255;
      FastSPI_LED.show();
      steparray ();
      if(dip() != 5){break;}
      delay (potValue0);
    }
  
    leds[0].r = 255;
    leds[0].g = 255;
    leds[0].b = 255 / benspeed;
    
    while(leds[0].r > 1){
      
      leds[0].r = leds[0].r / benspeed; //- potValue1;
      leds[0].g = leds[0].g / benspeed; //- potValue1;
      leds[0].b = leds[0].b / benspeed; //- potValue1/2;
      
      FastSPI_LED.show();
      steparray ();
      if(dip() != 5){break;}
      delay (potValue0);
    }
    
    memset(leds, 0, 3);
    
    for(int j = 0; j < (255/potValue1); j++){
      FastSPI_LED.show();
      steparray ();
      if(dip() != 5){break;}
      delay (potValue0);
    }
  }
    
//RANDOMIZER

  while( dip() == 6 ){
    
    memset(leds, 127, 3);
    randomSeed(analogRead(A5));
    
    while( dip() == 6 ){
      potValue0 = analogRead(pot0)/8;
      potValue1 = analogRead(pot1)/4;
      leds[0].r += random(-potValue0,potValue0);
      leds[0].g += random(-potValue0,potValue0);
      leds[0].b += random(-potValue0,potValue0);
      FastSPI_LED.show();
      steparray ();
      if(dip() != 6){break;}
      delay (potValue1);
    }
  }

//STANDING BY....
  while( dip() > 6 ){
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      memset(leds, 0, NUM_LEDS * 3);
      leds[i].g = 20;
      FastSPI_LED.show();
      if(dip() <= 6){break;}
      delay(50);
    }
  }


  
    
}

int dip (){
  return (digitalRead(aButton)+digitalRead(bButton)*2+digitalRead(cButton)*4+digitalRead(dButton)*8);
  //return(1);
}

void steparray (){
//    leds = leds + 2;
    for(int i = (NUM_LEDS-1); i > 0 ; i--){
    leds[i].r=leds[i-1].r;
    leds[i].g=leds[i-1].g;
    leds[i].b=leds[i-1].b;
    
  }
}


