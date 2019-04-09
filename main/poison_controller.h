#include "color_lib.h"
#include <Adafruit_DotStar.h>

class PoisonController {
    private:
        //Constants
        const int FRONTMODULE_LOWER_LIMIT = 0;
        const int FRONTMODULE_UPPER_LIMIT = 1;
        const int BACKMODULE_LOWER_LIMIT = 0;
        const int BACKMODULE_UPPER_LIMIT = 3;
        
        const uint32_t COLOR = 0x00330A;
        const double ONE_BRIGHTNESS_STEP = .009;    //speed at which the controller changes the brightness
        const double MAX_BRIGHTNESS = 1.0;
        const double MIN_BRIGHTNESS = 0.0;
        
        Adafruit_DotStar* strip;
        Adafruit_DotStar* stripBack;
        double currentBrightness = 0.0;
        int mode = 0;
    public:
        PoisonController(Adafruit_DotStar*, Adafruit_DotStar*);
        void loop();
        void setMode(int mode);
};

PoisonController::PoisonController(Adafruit_DotStar* injectedStrip, Adafruit_DotStar* injectedStripBack) 
{ 
    strip = injectedStrip; 
    stripBack = injectedStripBack;
}

//Modes:
//0: Off
//1: less poison
//2: more poison
void PoisonController::setMode(int mode)
{
    //off
    if (mode == 0) {
        currentBrightness = MIN_BRIGHTNESS; //turn off
    //less
    } else if (mode == 1) {
        currentBrightness -= ONE_BRIGHTNESS_STEP;
    //more
    } else if (mode == 2) {
        currentBrightness += ONE_BRIGHTNESS_STEP;
    }

    currentBrightness = currentBrightness > MIN_BRIGHTNESS ? currentBrightness : MIN_BRIGHTNESS;    //min bound
    currentBrightness = currentBrightness < MAX_BRIGHTNESS ? currentBrightness : MAX_BRIGHTNESS;    //max bound
}

void PoisonController::loop() {
    //Set color brightness
    CRGB color = COLOR;

    //Turn off
    if (currentBrightness - .00001 <= MIN_BRIGHTNESS) {
        color = 0x0;
    //Lower brightness
    } else {
        color %= (currentBrightness * 255);
    }

    setStripArrayColor(strip, rgbToHex(color), FRONTMODULE_LOWER_LIMIT, FRONTMODULE_UPPER_LIMIT);
    setStripArrayColor(stripBack, rgbToHex(color), BACKMODULE_LOWER_LIMIT, BACKMODULE_UPPER_LIMIT);
}