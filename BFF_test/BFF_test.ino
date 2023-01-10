// SPDX-FileCopyrightText: 2022 Phil B. for Adafruit Industries
//
// SPDX-License-Identifier: MIT

// Example for 5x5 NeoBFF - scrolls a message across the LED matrix.
// Requires Adafruit_GFX, Adafruit_NeoPixel and Adafruit_NeoMatrix libraries.

#include <Adafruit_GFX.h>       // Graphics library
#include <Adafruit_NeoPixel.h>  // NeoPixel library
#include <Adafruit_NeoMatrix.h> // Bridges GFX and NeoPixel
#include <Fonts/TomThumb.h>     // A tiny 3x5 font incl. w/GFX

#define PIN A3

// NeoMatrix declaration for BFF with the power and
// Neo pins at the top (same edge as QT Py USB port):
Adafruit_NeoMatrix piss(5, 5, PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB         + NEO_KHZ800);

// Message to display, and a set of colors to cycle through. Because
// the matrix is only 5 pixels tall, characters with descenders (e.g.
// lowercase p or y) are best avoided. There are even smaller fonts
// but these get progressively less legible. ALL CAPS helps!


// 
// 
bool check;
int ran;
int mssg_counter = 0;
int speed_counter = 60;
//String message[] = ;
//const char prt1[] = "- GOODNIGHT, STINKY!  ";
//const char prt2[] = "- I LOVE YOU!  ";
//const char prt3[] = "YOU JUST CHILLIN OR WHUT?   ";
//String message[] = {prt1, prt2, prt3};
const char prt1[] = "<<<<<<<<<<<<--------------";
const char prt2[] = "-------------->>>>>>>>>>>>";
const char prt3[] = "12345678901234567890123456";
const char prt4[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char prt5[] = "{}{}{}{}{}{}{}{}{}{}{}{}{}";
const char prt6[] = "||||||||||||||||||||||||||";
//char prt7[]
String message[] = {prt1, prt2, prt3, prt4, prt5, prt6};
//const lis

//const char message[] = "<<<<<<<<<<<<--------------"; // "<<------------<<<<<<<<<";
// use for blinkers ^^^


int randColor1;
int randColor2;
int randColor3;


const uint16_t colors[] = 
{
  piss.Color(255, 0, 0), piss.Color(255, 0, 255), piss.Color(0, 0, 255),
  piss.Color(0, 255, 255), piss.Color(0, 255, 0), piss.Color(255, 255, 0)
  };
  
uint16_t message_width; // Computed in setup() below

int bright = 100;


int16_t  d1;
uint16_t d2;
  
void setup() {
  //mssg_counter ++;
  check = false;
  piss.begin();
  piss.setBrightness(100);       // Turn down brightness to about 15%
  piss.setFont(&TomThumb);      // Use custom font
  piss.setTextWrap(false);      // Allows text to scroll off edges
  piss.setTextColor(colors[0]); // Start with first color in list
  // To determine when the message has fully scrolled off the left side,
  // get the bounding rectangle of the text. As we only need the width
  // value, a couple of throwaway variables are passed to the bounds
  // function for the other values:
  
  
  piss.getTextBounds(message[0], 0, 0, &d1, &d1, &message_width, &d2);
}

int x = piss.width();  // Start with message off right edge
int y = piss.height(); // With custom fonts, y is the baseline, not top
int pass = 0;            // Counts through the colors[] array

void loop() {
  randColor1 = random(255);
  randColor2 = random(255);
  randColor3 = random(255);
  
  piss.fillScreen(0);       // Erase message in old position.
  piss.setCursor(x, y);     // Set new cursor position,
  //piss.print(message[mssg_counter]);      // draw the message
  piss.print(message[ran]);
  piss.show();              // and update the matrix.
  if(--x < -message_width) {  // Move 1 pixel left. Then, if scrolled off left...
    //delay(2000);
    // delays message reset after scrolling has finished
    x = piss.width();       // reset position off right edge and
    
    //if(++pass >= 6) pass = 0; // increment color in list, rolling over if needed.
    //piss.setTextColor(colors[pass]);
    piss.setTextColor(piss.Color(randColor1,randColor2,randColor3));
    
    //mssg_counter +=1;
    ran = random(5);
  }
  if(mssg_counter >= 5){
    mssg_counter = 0;
  }
  piss.getTextBounds(message[mssg_counter], 0, 0, &d1, &d1, &message_width, &d2);
  //delay(100); // 1/10 sec pause
  //delay(200);  // affects scroll speed
  
  //speed_counter -= 1;
  
//  if(speed_counter <= 0){
//    speed_counter = 60; 
//  }


   if(!check){
    speed_counter -= 1;
   }else{
    speed_counter += 1;
   }
   
   delay(speed_counter);
   
   if(speed_counter >= 60){
    check = false;
   }else if(speed_counter <= 30){
    check = true;
   }
  
//  if(speed_counter < 60){
//    speed_counter += 1;
//  }else{
//    check = true;
//  }
  
  
  //delay(60);
  //// use for blinkers ^^^

  
  
  //pulsing effect(affects look is dependant on scroll speed. a good combo is delay of 60 and decrement interval of 10)
  bright -= 10;
  piss.setBrightness(bright);
  if (bright <= 0){
    bright = 100;
  }
// use for blinkers ^^^
}
// GOODNIGHT, STINKY!
// I LOVE YOU!
