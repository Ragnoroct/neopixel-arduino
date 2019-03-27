#include "color_lib.h"
#include <Adafruit_DotStar.h>

class HealthController {
    private:
        //Constants
        //Index definitions
        const int FRONTMODULE_LOWER_INDEX = 16;
        const int FRONTMODULE_UPPER_INDEX = 20;
        const int BACKMODULE_LOWER_INDEX = 0;
        const int BACKMODULE_UPPER_INDEX = 9;
        const uint32_t HEALTH_COLOR = 0xFF0000;
        const int MODE_0_TICK_TIMEOUT = 10;
        const int MODE_1_TICK_TIMEOUT = 150;
        const int MODE_2_TICK_TIMEOUT = 25;
        
        int healthCurrentPixel = FRONTMODULE_LOWER_INDEX;
        int backHealthCurrentPixel = BACKMODULE_LOWER_INDEX;
        int healthCounter = 0;
        int healthOn = true;
        
        Adafruit_DotStar* strip;
        Adafruit_DotStar* stripBack;
        int ticksTimout = MODE_0_TICK_TIMEOUT;
        int mode = 0;
    public:
        HealthController(Adafruit_DotStar*, Adafruit_DotStar*);
        void loop();
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
        ticksTimout = MODE_0_TICK_TIMEOUT;
        healthCounter = ticksTimout + 1;
    } else if (mode == 1) {
        ticksTimout = MODE_1_TICK_TIMEOUT;
        healthCounter = ticksTimout + 1;
    } else if (mode == 2) {
        ticksTimout = MODE_2_TICK_TIMEOUT;
        healthCounter = ticksTimout + 1;
    }
}

void HealthController::loop() {
    int tail = 0;
    int backTail = 0;
    uint32_t color;

    //Normal pulsing mode (0|1)
    if (mode == 0 || mode == 1) {
        if (counter(healthCounter, ticksTimout) == false) return;

        //front
        if (healthCurrentPixel == FRONTMODULE_LOWER_INDEX)
            tail = FRONTMODULE_UPPER_INDEX;
        else
            tail = healthCurrentPixel - 1;
        //back
        if (backHealthCurrentPixel == BACKMODULE_LOWER_INDEX)
            backTail = FRONTMODULE_UPPER_INDEX;
        else
            backTail = backHealthCurrentPixel - 1;

        //Set front module pixels
        strip->setPixelColor(tail, 0);   //turn last pixel off 
        strip->setPixelColor(healthCurrentPixel, HEALTH_COLOR);

        //Set back module pixels
        stripBack->setPixelColor(backTail, 0);   //turn last pixel off 
        stripBack->setPixelColor(backHealthCurrentPixel, HEALTH_COLOR);
        
        //front
        healthCurrentPixel++;
        if (healthCurrentPixel > FRONTMODULE_UPPER_INDEX)
            healthCurrentPixel = FRONTMODULE_LOWER_INDEX;
        //back
        backHealthCurrentPixel++;
        if (backHealthCurrentPixel > BACKMODULE_UPPER_INDEX)
            backHealthCurrentPixel = BACKMODULE_LOWER_INDEX;
    //Flashing mode (2)
    } else {
        if (counter(healthCounter, ticksTimout) == false) return;
        uint32_t color = healthOn ? HEALTH_COLOR : 0;
        //Front module
        for (int i = FRONTMODULE_LOWER_INDEX; i <= FRONTMODULE_UPPER_INDEX; i++) {
            strip->setPixelColor(i, color);
        }
        //Back module
        for (int i = BACKMODULE_LOWER_INDEX; i <= BACKMODULE_UPPER_INDEX; i++) {
            stripBack->setPixelColor(i, color);
        }
        healthOn = !healthOn;
    }
}
