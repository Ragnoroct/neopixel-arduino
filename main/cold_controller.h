#include "color_lib.h"
#include <Adafruit_DotStar.h>

class ColdController {
    private:
        //Constants
        const int BACKMODULE_UPPER_INDEX = 31;
        const int BACKMODULE_LOWER_INDEX = 38;
        const int FRONTMODULE_LOWER_INDEX = 5;
        const int FRONTMODULE_UPPER_INDEX = 9;

        const float BRIGHTNESS_STEPS[19] = { .1, .15, .2, .25, .3, .35, .4, .45, .5, .55, .6, .65, .7, .75, .8, .85, .9, .95, 1 };
        const uint32_t COLOR = 0x70DBDB;
        const int STEP_DURRATION = 50; //how many steps for whole animation
        
        Adafruit_DotStar* strip;
        int currentStep = 0;
        int currentBrightnessIndex = 0;
        int mode = 0;
    public:
        ColdController(Adafruit_DotStar*);
        void loop();
        void setMode(int mode);
};

ColdController::ColdController(Adafruit_DotStar* injectedStrip) 
{ 
    strip = injectedStrip; 
}

//Modes:
//0: Off
//1: less cold
//2: not implemented
void ColdController::setMode(int mode)
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

void ColdController::loop() {
    //mode off
    if (mode == 0) {
        setStripArrayColor(strip, 0x0, FRONTMODULE_LOWER_INDEX, FRONTMODULE_UPPER_INDEX);   //front
        setStripArrayColor(strip, 0x0, BACKMODULE_LOWER_INDEX, BACKMODULE_UPPER_INDEX);     //back
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

            setStripArrayColor(strip, rgbToHex(color), FRONTMODULE_LOWER_INDEX, FRONTMODULE_UPPER_INDEX);
            setStripArrayColor(strip, rgbToHex(color), BACKMODULE_LOWER_INDEX, BACKMODULE_UPPER_INDEX);

            currentStep++;
            if (currentStep > STEP_DURRATION) {
                currentStep = 0;
                currentBrightnessIndex++;
            }
        }
    }
}
