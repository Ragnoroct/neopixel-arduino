#include "color_lib.h"
#include <Adafruit_DotStar.h>

class HealthController {
    private:
        //Constants
        //Index definitions
        const int FRONTMODULE_LOWER_INDEX = 10;
        const int FRONTMODULE_UPPER_INDEX = 13;
        const int BACKMODULE_LOWER_INDEX = 23;
        const int BACKMODULE_UPPER_INDEX = 32;
        const uint32_t HEALTH_COLOR = 0xFF0000;
        const int MODE_0_TICK_TIMEOUT = 10;
        const int MODE_1_TICK_TIMEOUT = 150;
        const int MODE_2_TICK_TIMEOUT = 25;
        const int TICK_TIMEOUT_MODIFIER = 3;    //n times as long between switches
        
        int healthCurrentPixel = FRONTMODULE_LOWER_INDEX;
        int backHealthCurrentPixel = BACKMODULE_LOWER_INDEX;
        int healthCounter = 0;
        int backHealthCounter = 0;
        int healthOn = true;
        
        Adafruit_DotStar* strip;
        Adafruit_DotStar* stripBack;
        int ticksTimout = MODE_0_TICK_TIMEOUT * TICK_TIMEOUT_MODIFIER;
        int ticksTimoutBack = MODE_0_TICK_TIMEOUT;
        int mode = 0;
    public:
        HealthController(Adafruit_DotStar*, Adafruit_DotStar*);
        void loop();
        void runLoop(Adafruit_DotStar* strip, int &currentPixel, int &healthCounter, int ticksTimout, int upperIndex, int lowerIndex);
        void setMode(int mode);
};

HealthController::HealthController(Adafruit_DotStar* injectedStrip, Adafruit_DotStar* injectedStripBack) 
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
    if (mode == 0 || mode == 1) {
        if (counter(healthCounter, ticksTimout) == false) return;

        //front
        if (currentPixel == lowerIndex)
            tail = upperIndex;
        else
            tail = currentPixel - 1;

        Serial.println(currentPixel);
        
        //Set front module pixels
        strip->setPixelColor(tail, 0);   //turn last pixel off 
        strip->setPixelColor(currentPixel, HEALTH_COLOR);

        currentPixel++;
        if (currentPixel > upperIndex)
            currentPixel = lowerIndex;
    //Flashing mode (2)
    } else {
        if (counter(healthCounter, ticksTimout) == false) return;
        uint32_t color = healthOn ? HEALTH_COLOR : 0;

        for (int i = lowerIndex; i <= upperIndex; i++) {
            strip->setPixelColor(i, color);
        }
        healthOn = !healthOn;
    }
}

void HealthController::loop() {
    runLoop(strip, healthCurrentPixel, healthCounter, ticksTimout, FRONTMODULE_UPPER_INDEX, FRONTMODULE_LOWER_INDEX);
    runLoop(stripBack, backHealthCurrentPixel, backHealthCounter, ticksTimoutBack, BACKMODULE_UPPER_INDEX, BACKMODULE_LOWER_INDEX);
}
