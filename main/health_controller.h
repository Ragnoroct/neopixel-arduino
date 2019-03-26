#include "color_lib.h"
#include <Adafruit_DotStar.h>

class HealthController {
    private:
        //Constants
        //Index definitions
        int FRONTMODULE_LOWER_INDEX = 0;
        int FRONTMODULE_UPPER_INDEX = 4;
        int BACKMODULE_UPPER_INDEX = 30;
        int BACKMODULE_LOWER_INDEX = 21;
        
        int healthCurrentPixel = FRONTMODULE_LOWER_INDEX;
        int backHealthCurrentPixel = BACKMODULE_LOWER_INDEX;
        int healthCounter = 0;
        int healthOn = true;
        const uint32_t HEALTH_COLOR = 0xFF0000;
        
        const int LOWER_LIMIT = 12;
        const int UPPER_LIMIT = 17;
        const float BRIGHTNESS_STEPS[19] = { .1, .15, .2, .25, .3, .35, .4, .45, .5, .55, .6, .65, .7, .75, .8, .85, .9, .95, 1 };
        const uint32_t COLOR = 0x70DBDB;
        const int STEP_DURRATION = 50; //how many steps for whole animation
        
        Adafruit_DotStar* strip;
        int currentStep = 0;
        int currentBrightnessIndex = 0;
        int mode = 0;
    public:
        HealthController(Adafruit_DotStar*);
        void loop();
        void setMode(int mode);
};

HealthController::HealthController(Adafruit_DotStar* injectedStrip) 
{ 
    strip = injectedStrip; 
}

//Modes:
//0: Off
//1: normal heartbeat
//2: fast heartbeat
void HealthController::setMode(int mode)
{
    this->mode = mode;
    //less
    if (mode == 1) {
        currentStep = 0;    //reset to 0
        currentBrightnessIndex = 0;
    //more
    } else if (mode == 2) {
        currentStep = 0;
        currentBrightnessIndex = 9; //start halfway
    }
}

void HealthController::loop() {
    int tail = 0;
    int backTail = 0;
    uint32_t color;

    //Normal pulsing mode (0)
    if (mode == 0) {
        if (counter(healthCounter, 10) == false) return;

        //front
        if (healthCurrentPixel == FRONTMODULE_LOWER_INDEX)
            tail = FRONTMODULE_UPPER_INDEX;
        else
            tail = healthCurrentPixel - 1;
        if (backHealthCurrentPixel == BACKMODULE_LOWER_INDEX)
            backTail = FRONTMODULE_UPPER_INDEX;
        else
            backTail = backHealthCurrentPixel - 1;

        //Set front module pixels
        strip->setPixelColor(tail, 0);   //turn last pixel off 
        strip->setPixelColor(healthCurrentPixel, HEALTH_COLOR);

        //Set back module pixels
        strip->setPixelColor(backTail, 0);   //turn last pixel off 
        strip->setPixelColor(backHealthCurrentPixel, HEALTH_COLOR);
        
        healthCurrentPixel++;
        if (healthCurrentPixel > FRONTMODULE_UPPER_INDEX)
            healthCurrentPixel = FRONTMODULE_LOWER_INDEX;
        backHealthCurrentPixel++;
        if (backHealthCurrentPixel > BACKMODULE_UPPER_INDEX)
            backHealthCurrentPixel = BACKMODULE_LOWER_INDEX;
    //Flashing mode (1)
    } else {
        if (counter(healthCounter, 25) == false) return;
        uint32_t color = healthOn ? HEALTH_COLOR : 0;
        //Front module
        for (int i = FRONTMODULE_LOWER_INDEX; i <= FRONTMODULE_UPPER_INDEX; i++) {
            strip->setPixelColor(i, color);
        }
        //Back module
        for (int i = BACKMODULE_LOWER_INDEX; i <= BACKMODULE_UPPER_INDEX; i++) {
            strip->setPixelColor(i, color);
        }
        healthOn = !healthOn;
    }
}
