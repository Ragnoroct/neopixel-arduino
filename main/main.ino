#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <IRremote.h>
#include "color_lib.h"
#include "fire_controller.h"
#include "poison_controller.h"

#define NUMPIXELS 30 // Number of LEDs in strip
#define DATAPIN    3
#define CLOCKPIN   2
#define PLAY_PAUSE FFC23D

int IR_Pin = A0;
int IR_Value = 0;
int RECV_PIN = A5;
int BLUE_LED = 13;
int RED_LED = 12;

#define IR_CHANNEL_MINUS 0xFFA25D  
#define IR_CHANNEL 0xFF629D  
#define IR_CHANNEL_PLUS 0xFFE21D  
#define IR_BLUE_LED_OFF 0xFF22DD  
#define IR_BLUE_LED_ON 0xFF02FD  
#define IR_PLAYPAUSE 0xFFC23D  
#define IR_VOLUME_MINUS 0xFFE01F  
#define IR_VOLUME_PLUS 0xFFA857  
#define IR_EQ 0xFF906F  
#define IR_0 0xFF6897  
#define IR_100_PLUS 0xFF9867  
#define IR_200_PLUS 0xFFB04F  
#define IR_1 0xFF30CF  
#define IR_2 0xFF18E7  
#define IR_3 0xFF7A85  
#define IR_4 0xFF10EF  
#define IR_5 0xFF38C7  
#define IR_6 0xFF5AA5  
#define IR_7 0xFF42BD  
#define IR_8 0xFF4AB5  
#define IR_9 0xFF52AD  

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

//Controllers
FireController fireController = FireController(&strip);
PoisonController poisonController = PoisonController(&strip);

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

//Modes
int healthMode = 0;
int fireMode = 0;

int head  = 0;
int remoteAction = 0;
bool isPaused = 0;
bool increaseCold = false;
bool decreaseCold = false;

void translateIR(int * remoteAction) // takes action based on IR code received describing Car MP3 IR codes 
{
    switch(results.value) {
        case IR_CHANNEL_MINUS:
            break;
        case IR_CHANNEL:
            break;
        case IR_CHANNEL_PLUS:
            break;
        case IR_BLUE_LED_OFF:
            break;
        case IR_BLUE_LED_ON:
            break;
        //Pause loop
        case IR_PLAYPAUSE:
            isPaused ^= 1;
            break;
        //Lightning
        case IR_VOLUME_MINUS:
            break;
        case IR_VOLUME_PLUS:
            break;
        case IR_EQ:
            break;
        //Health
        case IR_0:
            healthMode = 0;
            break;
        case IR_100_PLUS:
            healthMode = 1;
            break;
        case IR_200_PLUS:
            break;
        //Cold
        case IR_1:
            decreaseCold = !decreaseCold;
            increaseCold = false;
            break;
        case IR_2:
            increaseCold = !increaseCold;
            decreaseCold = false;
            break;
        case IR_3:
            break;
        //Fire
        case IR_4:
            fireController.setMode(0);
            break;
        case IR_5:
            fireController.setMode(1);
            break;
        case IR_6:
            fireController.setMode(2);
            break;
        //Poison
        case IR_7:
            poisonController.setMode(0);
            break;
        case IR_8:
            poisonController.setMode(1);
            break;
        case IR_9:
            poisonController.setMode(2);
            break;
  }
} 


void stripLoop() {
    updateLightning(head);
    updateHealth(head);
    updateCold(head);
    fireController.loop();
    poisonController.loop();
    strip.show();                     // Refresh strip

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

int counterLastTime = 0;
bool counter(int &counter, int timeout)
{
    counter++;
    if (counter > timeout) {
        counter = 0;
        return true;
    } else {
        return false;
    }
}


int fHealth = 6;
int eHealth = 11;
int healthCurrentPixel = 0;
int healthCounter = 0;
int healthOn = true;
const uint32_t HEALTH_COLOR = strip.Color(255, 0, 0);
//Modes:
//0. back and forth
//1. flashing
void updateHealth(int pix) {
    int tail = 0;
    uint32_t color;

    //Normal pulsing mode (0)
    if (healthMode == 0) {
        if (counter(healthCounter, 10) == false) return;

        if (healthCurrentPixel == fHealth)
            tail = eHealth;
        else
            tail = healthCurrentPixel - 1;

        strip.setPixelColor(tail, 0);   //turn last pixel off
        strip.setPixelColor(healthCurrentPixel, HEALTH_COLOR);
        healthCurrentPixel++;
        if (healthCurrentPixel > eHealth)
            healthCurrentPixel = fHealth;
    //Flashing mode (1)
    } else {
        if (counter(healthCounter, 25) == false) return;
        uint32_t color = healthOn ? HEALTH_COLOR : 0;
        for (int i = fHealth; i <= eHealth; i++) {
            strip.setPixelColor(i, color);
        }
        healthOn = !healthOn;
    }
}

int fCold = 12;
int eCold = 17;
int coldCounter = 0;
uint32_t coldColor = strip.Color(17, 24, 24);//0x000000;
int coldColorTrack = 0; // If you add 0x11 to 0xFF, you will overflow, in order to keep track of "steps" of brightness, this variable will go from 0-16. This is a strong case to use your color operation -Jonathan
float coldStep = .0001;

void updateCold(int pix) {
   if (pix != fCold) return;
   if (increaseCold) {
      coldColorTrack += coldStep;
      coldColor += coldStep * coldColor;
      if (coldColorTrack > 16) {
        coldColorTrack = 16;
        coldColor = 0xA5F2F3;
      }
      for (int i = fCold; i <= eCold; i++) {
        strip.setPixelColor(i, coldColor);
      }
   } else if (decreaseCold) {
      coldColorTrack -= coldStep;
      coldColor -= coldStep * coldColor;
      if (coldColorTrack < 0) {
        coldColorTrack = 0;
        coldColor = 0x000000;
      }
      for (int i = fCold; i <= eCold; i++) {
        strip.setPixelColor(i, coldColor);
      }
   }
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
