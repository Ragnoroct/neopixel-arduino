#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <IRremote.h>
#include "color_lib.h"
#include "fire_controller.h"
#include "poison_controller.h"
#include "cold_controller.h"
#include "lightning_controller.h"
#include "health_controller.h"

#define NUMPIXELS_FRONT 21 // Number of LEDs in strip
#define NUMPIXELS_BACK 33 // Number of LEDs in strip
#define DATAPIN    13
#define CLOCKPIN   11
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
#define IR_REPEAT 0xFFFFFFFF

// Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS_FRONT, 2, 3, DOTSTAR_BGR);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS_FRONT, 3, NEO_GRB + NEO_KHZ800);
Adafruit_DotStar backStrip = Adafruit_DotStar(NUMPIXELS_BACK, DOTSTAR_BGR);

//Controllers
FireController fireController = FireController(&strip, &backStrip);
PoisonController poisonController = PoisonController(&strip, &backStrip);
ColdController coldController = ColdController(&strip, &backStrip);
LightningController lightningController = LightningController(&strip, &backStrip);
HealthController healthController = HealthController(&strip, &backStrip);

IRrecv irrecv(RECV_PIN);
decode_results results;
uint32_t lastIRValue;

void setup() {
  backStrip.begin();
  backStrip.show();
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

int remoteAction = 0;
bool isPaused = 0;

void translateIR(int * remoteAction) // takes action based on IR code received describing Car MP3 IR codes 
{
    if (results.value == IR_REPEAT) {
        results.value = lastIRValue;
    } else if (results.value != IR_PLAYPAUSE) {
        lastIRValue = results.value;
    }

    Serial.println(results.value, HEX);
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
        //Pause/play loop
        case IR_PLAYPAUSE:
            isPaused = isPaused ? false : true;
            break;
        //Lightning
        case IR_VOLUME_MINUS:
            lightningController.setMode(0);
            break;
        case IR_VOLUME_PLUS:
            lightningController.setMode(1);
            break;
        case IR_EQ:
            lightningController.setMode(2);
            break;
        //Health
        case IR_0:
            healthController.setMode(0);
            break;
        case IR_100_PLUS:
            healthController.setMode(1);
            break;
        case IR_200_PLUS:
            healthController.setMode(2);
            break;
        //Cold
        case IR_1:
            coldController.setMode(0);
            break;
        case IR_2:
            coldController.setMode(1);
            break;
        case IR_3:
            coldController.setMode(2);
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
    if (isPaused == false) {
        lightningController.loop();
        healthController.loop();
        coldController.loop();
        fireController.loop();
        poisonController.loop();
    } else {
        offLoop();
    }

    // Refresh strips
    backStrip.show();
    strip.show();                    
}

void offLoop() {
  for (int i = 0; i < NUMPIXELS_FRONT; i++) {
    strip.setPixelColor(i, 0x0);
  }
  for (int i = 0; i < NUMPIXELS_BACK; i++) {
    backStrip.setPixelColor(i, 0x0);
  }
}

void loop() {
    //parse remote
    while (!irrecv.isIdle());  // if not idle, wait till complete
    if (irrecv.decode(&results)) {
        translateIR(&remoteAction);
        irrecv.resume(); // Receive the next value
    }

    //main pixel logic
    stripLoop();
}
