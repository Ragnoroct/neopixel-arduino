#include "color_lib.h"
#include <Adafruit_DotStar.h>

class LightningController {
    private:
        //Constants
        const int LOWER_LIMIT = 0;
        const int UPPER_LIMIT = 5;
        const float BRIGHTNESS_STEPS[8] = { 1, 0, .8, 0, .99, 0, .7, 0 };
        const uint32_t COLOR = 0x70DBDB;
        const int STEP_DURRATION = 10; //how many steps for change in animation
        
        Adafruit_DotStar* strip;
        int currentStep = 0;
        int currentBrightnessIndex = 0;
        int mode = 0;
    public:
        LightningController(Adafruit_DotStar*);
        void loop();
        void setMode(int mode);
};

LightningController::LightningController(Adafruit_DotStar* injectedStrip) 
{ 
    strip = injectedStrip; 
}

//Modes:
//0: Off
//1: lightning loop
//2: not implemented
void LightningController::setMode(int mode)
{
    this->mode = mode;
    //lightning loop
    if (mode == 1) {
        currentStep = 0;    //reset to 0
        currentBrightnessIndex = 0;
    }
}

void LightningController::loop() {
    //mode off
    if (mode == 0) {
        for (int i = LOWER_LIMIT; i <= UPPER_LIMIT; i++) {
            strip->setPixelColor(i, 0);
        }
    //mode less
    } else if (mode == 1) {
        //only step up in brightness to 9
        if (currentBrightnessIndex < 8) {
            // int index = currentBrightnessIndex;
            float colorCoefficient = BRIGHTNESS_STEPS[currentBrightnessIndex];
            // float extraCoefficient = BRIGHTNESS_STEPS[index + 1];   //between index and index + 1
            // extraCoefficient = (extraCoefficient - colorCoefficient) * (currentStep / STEP_DURRATION);
            // colorCoefficient += extraCoefficient;
            
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
        } else {
            //reset loop
            currentBrightnessIndex = 0;
            currentStep = 0;
        }
    }
}

// int fLightning = 0;
// int eLightning = 5;
// int lightningCounter = 0;
// int lightningT = 10;
// void updateLightning(int pix) {
//    if (pix != fLightning) return;
//    if (++lightningCounter == lightningT) {
//       for (int i = fLightning; i <= eLightning; i++) {
//         strip.setPixelColor(i, 0x00FFFF);
//       }
//       lightningCounter = 0;
//    } else if (lightningCounter == lightningT/2) {
//       for (int i = fLightning; i <= eLightning; i++) {
//         strip.setPixelColor(i, 0);
//     }
//    }
// }