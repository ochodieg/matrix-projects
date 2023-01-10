/*
 * 
 */

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h> 


#define dOut A3

Adafruit_NeoMatrix bff = Adafruit_NeoMatrix(5, 5, dOut,
  NEO_MATRIX_TOP  + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB         + NEO_KHZ800);

// no brightness control in NeoMatrix library, so need to create neopixel object
Adafruit_NeoPixel bff2(25, dOut, NEO_GRB + NEO_KHZ800);
  // no brightness control in NeoMatrix library, so need to create neopixel object

  
uint16_t  mssg_w;   // getTextBounds() -> width
uint16_t  mssg_h;   // getTextBounds() -> height
int16_t   origin;   // getTextBounds() -> x2, y2

int x   = bff.width();// matrix width
//int x2  = bff.width();// matrix width for input (not needed)
int y   = bff.height();//matrix height

// random color values
int randC1;
int randC2;
int randC3;
//random mssg select
int ranWord;

String message[] = { "<<<<<<<<<<<<<<<<----------",
                     "---------->>>>>>>>>>>>>>>>",
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                     "0 1 2 3 4 5 6 7 8 9 10 11 12",
                     "! @ # $ % ^ & * { } < > ? ! +",
                     "TyPe something  ..."};
String words;       // used for input
bool hasWords;      // used for input
String rainCheck = "rainbow";

bool speedCheck;
int speedCounter;
int bright;

void setup() {
  randC1 = random(255);
  randC2 = random(255);
  randC3 = random(255);
  hasWords = true;
  Serial.begin(9600);
  bff.begin();
  bff.setBrightness(40);
  bff.setTextWrap(false);
  bff.setFont(&TomThumb);
  bff.fillScreen(0);  // clear screen
  
  cWipe(0x0000ff, 50);
  cWipeR(0xff0000, 50);
  cWipe(0x00ff00, 50);
  pixelTravel(0xf800);    //16 bit
  rainbowFade2White(3, 6, 0); // 32 bit
  
  bff.fillScreen(0);
  bff.setCursor(x,y);
  //bff.setTextColor(bff.Color(randC1, randC2, randC3));
  bff.setTextColor(bff.Color(255, 0, 0));
  
  speedCheck = false;
  speedCounter = 60;
  bright = 100;

  // after initialization
  // & set up serial comms,
  while(!Serial); // while serial is not available, do nothing 
  Serial.println("mssg_w and mssg_h are mutated by get TextBounds()");
  


  
}

void loop() {
  randC1 = random(255);
  randC2 = random(255);
  randC3 = random(255);

  bff.getTextBounds(message[ranWord],
                    x, // as x decrements, the bounds change 
                    y,
                    &origin,
                    &origin,
                    &mssg_w,
                    &mssg_h);
 
  bff.fillScreen(0);
  bff.setCursor(x,y); // cursor updates with x-decrement and "scrolls" text       
  bff.print(message[ranWord]);
  bff.show();
  if(--x < -mssg_w){
    x = bff.width();
    ranWord= random(6);
    bff.setTextColor(bff.Color(randC1, randC2, randC3));
  }

  if(Serial.available() > 0){             // ensure serial is writable
    words = Serial.readStringUntil('\n'); // optain serial input until newline
    words.trim();
    if(words.equalsIgnoreCase(rainCheck)){
      Serial.println("playing rainbow animtation");
      rainbowFade2White(3,6,0);
    }else{
      Serial.print("Displaying: ");
      Serial.println(words);
      Serial.print("Word length is: ");
      Serial.println(words.length());
      bff.setBrightness(100);
      x = bff.width();                      // reset x position for new message
      bff.getTextBounds(words,              // get bounds of input
                      x,
                      y,
                      &origin,
                      &origin,
                      &mssg_w,
                      &mssg_h);
      hasWords = true;                      // flag that we have an input
      while(hasWords){                      // scroll input then continue
        bff.fillScreen(0);
        bff.setCursor(x,y);
        bff.print(words);
        bff.show();
        delay(150);
       if(--x < -mssg_w){
          x = bff.width();
         hasWords = false;
        }
      }      
    }
  }



   if(!speedCheck){
    speedCounter -= 1;
   }else{
    speedCounter += 1;
   }
   
   delay(speedCounter);
   
   if(speedCounter >= 60){
    speedCheck = false;
   }else if(speedCounter <= 30){
    speedCheck = true;
   } 
  bright -= 10;
  bff.setBrightness(bright);
  if (bright <= 0){
    bright = 100;
  }    
  
   //delay(70);
  //delay(20);
}


/*
 * param@ color: color chosen
 * param@ wait: speed of wipe
 * pixels color is stored in RAM
 * 
 * because this method is using specific
 * array coordinates and is linearly  
 * progressing, cursor does not need to be set
 * 
 */
void cWipe(uint32_t color, int wait){
  for(int i = 0; i<bff.numPixels(); i++){
    bff.setPixelColor(i, color);
    bff.show();
    delay(wait);
  }
}
void cWipeR(uint32_t color, int wait){
  for(int i = bff.numPixels(); i>=0; i--){
    bff.setPixelColor(i, color);
    bff.show();
    delay(wait);
  }
}

//void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops){
//  int fadeVal = 0, fadeMax=100;
//
//  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
//  firstPixelHue += 256){
//    for(int i=0; i<
//  }
//}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {
    for(int i=0; i<bff.numPixels(); i++) { // For each pixel in bff...
      uint32_t pixelHue = firstPixelHue + (i * 65536L / bff.numPixels());
      bff.setPixelColor(i, bff.gamma32(bff.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }
    bff.show();
    delay(wait);
    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }
  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire bff with white at gamma-corrected brightness level 'j':
      //bff.fill(bff.Color(0, 0, 0, bff.gamma8(j)));
      bff.fill(bff.Color(0, 0, 255)); // Color() does not support 4 args in NeoMatrix
      bff.show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      //bff.fill(bff.Color(0, 0, 0, bff.gamma8(j)));
      bff.fill(bff.Color(0, 0, 255)); // Color() does not support 4 args in NeoMatrix
      bff.show();
    }
  }
  delay(500); // Pause 1/2 second
}

void pixelTravel(uint16_t color){
  bff.fill(0);
  bff.setCursor(x,y);
  for(int i = 0; i<5; i++){
    for(int j = 0; j<5; j++){
      //bff2.setPixelColor(j+1,0x800000);    // doesn't work
      bff.drawPixel(j, i, color);
      //bff2.setPixelColor(j-1,0x800000);    // doesn't work 
      bff.show();
      //bff2.show();
      delay(50);
      bff.fill(0);
    }
  } 
}
