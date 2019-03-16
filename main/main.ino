
#ifndef SIMULATION
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <IRremote.h>
#endif

#include "controllers.h"

#define NUMPIXELS 60 // Number of LEDs in strip
#define DATAPIN    1
#define CLOCKPIN   0
#define PLAY_PAUSE FFC23D

int IR_Pin = A0;
int IR_Value = 0;
int RECV_PIN = A5;
int BLUE_LED = 13;
int RED_LED = 12;

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.
// HealthController healthController = HealthController(&strip);

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  pinMode(RECV_PIN, INPUT);  
  pinMode(BLUE_LED, OUTPUT); 
  pinMode(RED_LED, OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
//  Serial.begin(9600);
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

// int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
// uint32_t color = 0xFF0000;      // 'On' color (starts red)
int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)
int remoteAction = 0;
bool isPaused = 0;

void healthLoop() {
  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
      color = 0xFF0000;             //   Yes, reset to red
  }
  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
}

void poisonLoop() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0x49B960);
  }
}

void offLoop() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0x0);
  }
  strip.show();
}

void loop() {
  // int i=0;
  // if (irrecv.decode(&results)) {
  //   translateIR(&remoteAction);
  //   irrecv.resume(); // Receive the next value
  // }

  // if (isPaused) {
  //   offLoop();
  //   return;
  // }
    
//    ir = analogRead(IR_Pin); 
//  strip.setBrightness(1);
  strip.setPixelColor(1, 0xFFFFFF);
  strip.show();
  // healthController.loop();
  // Serial.println(healthController);
  // healthLoop();
//  IR_Value = analogRead(IR_Pin);
  // healthLoop();
//  Serial.println("delay value for LED = "+ IR_Value);
//  strip.setPixelColor(head, color); // 'On' pixel at head
//  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
//  strip.show();                     // Refresh strip
//  delay(20);                        // Pause 20 milliseconds (~50 FPS)
//
//  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
//    head = 0;                       //  Yes, reset head index to start
//    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
//      color = 0xFF0000;             //   Yes, reset to red
//  }
//  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
}

void translateIR(int * remoteAction) // takes action based on IR code received describing Car MP3 IR codes 
{
  switch(results.value) {
    case 0xFFC23D:
      isPaused ^= 1;  // Play or pause
  }

  delay(500);


} 
