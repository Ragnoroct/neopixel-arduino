#include "color_lib.h"
#include <Adafruit_DotStar.h>

class LightningController {
    private:
        //Constants
        const int FRONTMODULE_LOWER_LIMIT = 10;
        const int FRONTMODULE_UPPER_LIMIT = 13;
        const int BACKMODULE_LOWER_LIMIT = 39;
        const int BACKMODULE_UPPER_LIMIT = 44;

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
        setStripArrayColor(strip, 0x0, FRONTMODULE_LOWER_LIMIT, FRONTMODULE_UPPER_LIMIT);
        setStripArrayColor(strip, 0x0, BACKMODULE_LOWER_LIMIT, BACKMODULE_UPPER_LIMIT);
    //mode less
    } else if (mode == 1) {
        //only step up in brightness to 9
        if (currentBrightnessIndex < 8) {
            float colorCoefficient = BRIGHTNESS_STEPS[currentBrightnessIndex];
            
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