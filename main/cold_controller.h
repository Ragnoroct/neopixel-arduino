#include "color_lib.h"
#include <Adafruit_DotStar.h>

class ColdController {
    private:
        //Constants
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
        for (int i = LOWER_LIMIT; i <= UPPER_LIMIT; i++) {
            strip->setPixelColor(i, 0);
        }
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

            for (int i = LOWER_LIMIT; i <= UPPER_LIMIT; i++) {
                strip->setPixelColor(i, rgbToHex(color));
            }

            currentStep++;
            if (currentStep > STEP_DURRATION) {
                currentStep = 0;
                currentBrightnessIndex++;
            }
        }
    }
}

// int fCold = 12;
// int eCold = 17;
// int coldCounter = 0;
// uint32_t coldColor = strip.Color(17, 24, 24);//0x000000;
// int coldColorTrack = 0; // If you add 0x11 to 0xFF, you will overflow, in order to keep track of "steps" of brightness, this variable will go from 0-16. This is a strong case to use your color operation -Jonathan
// float coldStep = .0001;

// void updateCold(int pix) {
//    if (pix != fCold) return;
//    if (increaseCold) {
//       coldColorTrack += coldStep;
//       coldColor += coldStep * coldColor;
//       if (coldColorTrack > 16) {
//         coldColorTrack = 16;
//         coldColor = 0xA5F2F3;
//       }
//       for (int i = fCold; i <= eCold; i++) {
//         strip.setPixelColor(i, coldColor);
//       }
//    } else if (decreaseCold) {
//       coldColorTrack -= coldStep;
//       coldColor -= coldStep * coldColor;
//       if (coldColorTrack < 0) {
//         coldColorTrack = 0;
//         coldColor = 0x000000;
//       }
//       for (int i = fCold; i <= eCold; i++) {
//         strip.setPixelColor(i, coldColor);
//       }
//    }
// }