#include <Adafruit_DotStar.h>
#include <SPI.h>

#define NUMPIXELS 60 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    1
#define CLOCKPIN   0
int IR_Pin = A0;
int IR_Value = 0;
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
//  Serial.begin(115200);
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

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

void loop() {
//  strip.setBrightness(1);
  healthLoop();
//  IR_Value = analogRead(IR_Pin);
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
