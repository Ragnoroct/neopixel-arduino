
#ifndef SIMULATION
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <IRremote.h>
#endif

#define NUMPIXELS 30 // Number of LEDs in strip
#define DATAPIN    3
#define CLOCKPIN   2
#define PLAY_PAUSE FFC23D

int IR_Pin = A0;
int IR_Value = 0;
int RECV_PIN = A5;
int BLUE_LED = 13;
int RED_LED = 12;

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  pinMode(RECV_PIN, INPUT);  
  pinMode(BLUE_LED, OUTPUT); 
  pinMode(RED_LED, OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
  Serial.begin(9600);
}

int head  = 0;
int remoteAction = 0;
bool isPaused = 0;

void translateIR(int * remoteAction) // takes action based on IR code received describing Car MP3 IR codes 
{
  switch(results.value) {
    case 0xFFC23D:
      isPaused ^= 1;  // Play or pause
  }
  delay(500);
} 

void stripLoop() {
  updateLightning(head);
  updateHealth(head);
  updateCold(head);
  updateFire(head);
  updatePoison(head);
  strip.show();                     // Refresh strip
  delay(5);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= NUMPIXELS) head = 0;
}

int fLightning = 0;
int eLightning = 5;
int lightningCounter = 0;
int lightningT = 10;
void updateLightning(int pix) {
   if (pix != fLightning) return;
   if (++lightningCounter == lightningT) {
      for (int i = fLightning; i <= eLightning; i++) {
        strip.setPixelColor(i, 0x00FFFF);
      }
      lightningCounter = 0;
   } else if (lightningCounter == lightningT/2) {
      for (int i = fLightning; i <= eLightning; i++) {
        strip.setPixelColor(i, 0);
    }
   }
}

int fHealth = 6;
int eHealth = 11;
void updateHealth(int pix) {
   if (pix < fHealth || pix > eHealth) return;
   strip.setPixelColor(pix, 0x00FF00);
}

int fCold = 12;
int eCold = 17;
int coldT = 3;
int coldCounter = 0;
void updateCold(int pix) {
   if (pix < fCold || pix > eCold) return;
   strip.setPixelColor(pix, 0xFF0000);
}

int fFire = 18;
int eFire = 23;
void updateFire(int pix) {
   if (pix < fFire || pix > eFire) return;
   strip.setPixelColor(pix, 0x00FFA5);
}

int fPoison = 24;
int ePoison = 29;
void updatePoison(int pix) {
   if (pix < fPoison || pix > ePoison) return;
   strip.setPixelColor(pix, 0x0000FF);
}

void offLoop() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0x0);
  }
  strip.show();
}

void loop() {
   int i=0;
   if (irrecv.decode(&results)) {
     translateIR(&remoteAction);
     irrecv.resume(); // Receive the next value
   }

   if (isPaused) {
    offLoop();
    return;
   }
  stripLoop();
}
