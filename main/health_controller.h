#include "color_lib.h"
#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>

class HealthController {
    private:
        //Constants
        //Index definitions
        const int FRONTMODULE_LOWER_INDEX = 10;
        const int FRONTMODULE_UPPER_INDEX = 13;
        const int BACKMODULE_LOWER_INDEX = 23;
        const int BACKMODULE_UPPER_INDEX = 32;
        const uint32_t HEALTH_COLOR = 0xFF0000;
        const int MODE_0_TICK_TIMEOUT = 25; //normal (slow)
        const int MODE_1_TICK_TIMEOUT = 10; //faster
        const int MODE_2_TICK_TIMEOUT = 25;
        const int TICK_TIMEOUT_MODIFIER = 2.5;    //n times as long between switches
        
        int healthCurrentPixel = FRONTMODULE_LOWER_INDEX;
        int backHealthCurrentPixel = BACKMODULE_LOWER_INDEX;
        int healthCounter = 0;
        int backHealthCounter = 0;
        int healthOn = true;
        
        Adafruit_NeoPixel* strip;
        Adafruit_DotStar* stripBack;
        int ticksTimout = MODE_0_TICK_TIMEOUT * TICK_TIMEOUT_MODIFIER;
        int ticksTimoutBack = MODE_0_TICK_TIMEOUT;
        int mode = 0;
    public:
        HealthController(Adafruit_NeoPixel*, Adafruit_DotStar*);
        void loop();
        void runLoop(Adafruit_DotStar* strip, int &currentPixel, int &healthCounter, int ticksTimout, int upperIndex, int lowerIndex);
        void runLoop(Adafruit_NeoPixel* strip, int &currentPixel, int &healthCounter, int ticksTimout, int upperIndex, int lowerIndex);
        void setMode(int mode);
};

HealthController::HealthController(Adafruit_NeoPixel* injectedStrip, Adafruit_DotStar* injectedStripBack) 
{ 
    strip = injectedStrip;
    stripBack = injectedStripBack;
}

//Modes:
//0: Off
//1: normal heartbeat
//2: fast heartbeat
void HealthController::setMode(int mode)
{
    this->mode = mode;
    if (mode == 0) {
        ticksTimout = MODE_0_TICK_TIMEOUT * TICK_TIMEOUT_MODIFIER;
        ticksTimoutBack = MODE_0_TICK_TIMEOUT;
        healthCounter = ticksTimout + 1;
    } else if (mode == 1) {
        ticksTimout = MODE_1_TICK_TIMEOUT * TICK_TIMEOUT_MODIFIER;
        ticksTimoutBack = MODE_1_TICK_TIMEOUT;
        healthCounter = ticksTimout + 1;
    } else if (mode == 2) {
        ticksTimout = MODE_2_TICK_TIMEOUT;
        ticksTimoutBack = MODE_2_TICK_TIMEOUT;
        healthCounter = ticksTimout + 1;
    }
}

void HealthController::runLoop(Adafruit_DotStar* strip, int &currentPixel, int &healthCounter, int ticksTimout, int upperIndex, int lowerIndex)
{
    int tail;

    //Normal pulsing mode (0|1)
    if (counter(healthCounter, ticksTimout) == false) return;

    //front
    if (currentPixel == lowerIndex)
        tail = upperIndex;
    else
        tail = currentPixel - 1;

    //Set front module pixels
    strip->setPixelColor(tail, 0);   //turn last pixel off 
    strip->setPixelColor(currentPixel, HEALTH_COLOR);

    currentPixel++;
    if (currentPixel > upperIndex)
        currentPixel = lowerIndex;
}

void HealthController::runLoop(Adafruit_NeoPixel* strip, int &currentPixel, int &healthCounter, int ticksTimout, int upperIndex, int lowerIndex)
{
    int tail;

    //Normal pulsing mode (0|1)
    if (counter(healthCounter, ticksTimout) == false) return;

    //front
    if (currentPixel == lowerIndex)
        tail = upperIndex;
    else
        tail = currentPixel - 1;

    //Set front module pixels
    strip->setPixelColor(tail, 0);   //turn last pixel off 
    strip->setPixelColor(currentPixel, HEALTH_COLOR);

    currentPixel++;
    if (currentPixel > upperIndex)
        currentPixel = lowerIndex;
}

void HealthController::loop() {
    if (mode != 2) {
        runLoop(strip, healthCurrentPixel, healthCounter, ticksTimout, FRONTMODULE_UPPER_INDEX, FRONTMODULE_LOWER_INDEX);
        runLoop(stripBack, backHealthCurrentPixel, backHealthCounter, ticksTimoutBack, BACKMODULE_UPPER_INDEX, BACKMODULE_LOWER_INDEX);
    //Flashing mode (2)
    } else {
        if (counter(healthCounter, ticksTimout) == false) return;
        uint32_t color = healthOn ? HEALTH_COLOR : 0;

        for (int i = FRONTMODULE_LOWER_INDEX; i <= FRONTMODULE_UPPER_INDEX; i++) {
            strip->setPixelColor(i, color);
        }
        for (int i = BACKMODULE_LOWER_INDEX; i <= BACKMODULE_UPPER_INDEX; i++) {
            stripBack->setPixelColor(i, color);
        }
        healthOn = !healthOn;
    }
}
