#include "color_lib.h"
#include <Adafruit_DotStar.h>

class FireController {
    private:
        Adafruit_DotStar* strip;
        int fFire = 18;
        int eFire = 23;
        float fireStep[10] = { .1, .2, .3, .4, .5, .6, 1 };
        uint32_t fireColor = 0xed310b;
        int fireDuration = 400; //how many steps for whole animation
        int fireStepsPerTransition = fireDuration / (sizeof(fireStep)/sizeof(fireStep[0]));
        int fireCurrentStep = 0;
    public:
        int fireMode = 0;
        FireController(Adafruit_DotStar*);
        void loop();
        void setMode(int mode);
};

FireController::FireController(Adafruit_DotStar* injectedStrip) 
{ 
    strip = injectedStrip; 
}

//Modes:
//0: Off
//1: off -> on
//2: on -> off
void FireController::setMode(int mode)
{
    fireMode = mode;
    if (mode == 1) {
        fireCurrentStep = 0;    //reset to 0
    } else if (mode == 2) {
        fireCurrentStep = fireDuration;
    }
}

void FireController::loop() {
    if (fireMode == 0) {
        for (int i = fFire; i <= eFire; i++) {
            strip->setPixelColor(i, 0);
        }
    } else if (fireMode == 1) {
        if (fireCurrentStep < fireDuration) {
            int stepCoefficientIndex = fireCurrentStep / fireStepsPerTransition;
            float colorCoefficient = fireStep[stepCoefficientIndex] + (.01 * (fireCurrentStep % fireStepsPerTransition));

            CRGB color = fireColor;
            color %= (colorCoefficient * 255);

            for (int i = fFire; i <= eFire; i++) {
                strip->setPixelColor(i, rgbToHex(color));
            }
            fireCurrentStep++;
        } else {
            fireCurrentStep = 0;
        }
    } else if (fireMode == 2) {
        if (fireCurrentStep < 0) {
            int stepCoefficientIndex = fireCurrentStep / fireStepsPerTransition;
            float colorCoefficient = fireStep[stepCoefficientIndex] + (.01 * (fireCurrentStep % fireStepsPerTransition));

            CRGB color = fireColor;
            color %= (colorCoefficient * 255);

            for (int i = fFire; i <= eFire; i++) {
                strip->setPixelColor(i, rgbToHex(color));
            }
            fireCurrentStep--;
        } else {
            fireMode = 0;   //off
        }
    }
}