#include "color_lib.h"
#include <Adafruit_DotStar.h>

class FireController {
    private:
        Adafruit_DotStar* strip;
        Adafruit_DotStar* stripBack;
        const int FRONTMODULE_LOWER_LIMIT = 3;
        const int FRONTMODULE_UPPER_LIMIT = 6;
        const int BACKMODULE_LOWER_LIMIT = 24;
        const int BACKMODULE_UPPER_LIMIT = 28;

        float fireStep[10] = { .1, .2, .3, .4, .5, .6, 1 };
        uint32_t fireColor = 0xed310b;
        int fireDuration = 400; //how many steps for whole animation
        int fireStepsPerTransition = fireDuration / (sizeof(fireStep)/sizeof(fireStep[0]));
        int fireCurrentStep = 0;
    public:
        int fireMode = 0;
        FireController(Adafruit_DotStar*, Adafruit_DotStar*);
        void loop();
        void setMode(int mode);
};

FireController::FireController(Adafruit_DotStar* injectedStrip, Adafruit_DotStar* injectedStripBack) 
{ 
    strip = injectedStrip;
    stripBack = injectedStripBack;
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
        setStripArrayColor(strip, 0X0, FRONTMODULE_LOWER_LIMIT, FRONTMODULE_UPPER_LIMIT);
        setStripArrayColor(strip, 0X0, BACKMODULE_LOWER_LIMIT, BACKMODULE_UPPER_LIMIT);
    } else if (fireMode == 1) {
        if (fireCurrentStep < fireDuration) {
            int stepCoefficientIndex = fireCurrentStep / fireStepsPerTransition;
            float colorCoefficient = fireStep[stepCoefficientIndex] + (.01 * (fireCurrentStep % fireStepsPerTransition));

            CRGB color = fireColor;
            color %= (colorCoefficient * 255);

            setStripArrayColor(strip, rgbToHex(color), FRONTMODULE_LOWER_LIMIT, FRONTMODULE_UPPER_LIMIT);
            setStripArrayColor(strip, rgbToHex(color), BACKMODULE_LOWER_LIMIT, BACKMODULE_UPPER_LIMIT);

            fireCurrentStep++;
        } else {
            fireCurrentStep = 0;
        }
    } else if (fireMode == 2) {
        setStripArrayColor(strip, fireColor, FRONTMODULE_LOWER_LIMIT, FRONTMODULE_UPPER_LIMIT);
        setStripArrayColor(stripBack, fireColor, BACKMODULE_LOWER_LIMIT, BACKMODULE_UPPER_LIMIT);
    }
}