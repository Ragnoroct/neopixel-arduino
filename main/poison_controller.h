#include "color_lib.h"
#include <Adafruit_DotStar.h>

class PoisonController {
    private:
        //Constants
        const int FRONTMODULE_LOWER_LIMIT = 18;
        const int FRONTMODULE_UPPER_LIMIT = 20;
        const int BACKMODULE_LOWER_LIMIT = 50;
        const int BACKMODULE_UPPER_LIMIT = 53;
        
        const float BRIGHTNESS_STEPS[19] = { .1, .15, .2, .25, .3, .35, .4, .45, .5, .55, .6, .65, .7, .75, .8, .85, .9, .95, 1 };
        const uint32_t COLOR = 0x00330A;
        const int STEP_DURRATION = 50; //how many steps for whole animation
        
        Adafruit_DotStar* strip;
        int currentStep = 0;
        int currentBrightnessIndex = 0;
        int mode = 0;
    public:
        PoisonController(Adafruit_DotStar*);
        void loop();
        void setMode(int mode);
};

PoisonController::PoisonController(Adafruit_DotStar* injectedStrip) 
{ 
    strip = injectedStrip; 
}

//Modes:
//0: Off
//1: less poison
//2: more poison
void PoisonController::setMode(int mode)
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

void PoisonController::loop() {
    //mode off
    if (mode == 0) {
        setStripArrayColor(strip, 0x0, FRONTMODULE_LOWER_LIMIT, FRONTMODULE_UPPER_LIMIT);
        setStripArrayColor(strip, 0x0, BACKMODULE_LOWER_LIMIT, BACKMODULE_UPPER_LIMIT);
    //mode less
    } else if (mode == 1) {
        //only step up in brightness to 9
        if (currentBrightnessIndex < 9) {
            int index = currentBrightnessIndex;
            float colorCoefficient = BRIGHTNESS_STEPS[index];
            float extraCoefficient = BRIGHTNESS_STEPS[index + 1];   //between index and index + 1
            extraCoefficient = (extraCoefficient - colorCoefficient) * (currentStep / STEP_DURRATION);
            colorCoefficient += extraCoefficient;
            
            //Set color brightness
            CRGB color = COLOR;
            color %= (colorCoefficient * 255);

            setStripArrayColor(strip, rgbToHex(color), FRONTMODULE_LOWER_LIMIT, FRONTMODULE_UPPER_LIMIT);
            setStripArrayColor(strip, rgbToHex(color), BACKMODULE_LOWER_LIMIT, BACKMODULE_UPPER_LIMIT);

            currentStep++;
            if (currentStep > STEP_DURRATION) {
                currentStep = 0;
                currentBrightnessIndex++;
            }
        }
    }
}