#include "color_lib.h"
#include <Adafruit_DotStar.h>

class ColdController {
    private:
        //Constants
        const int FRONTMODULE_LOWER_LIMIT = 18;
        const int FRONTMODULE_UPPER_LIMIT = 20;
        const int BACKMODULE_LOWER_LIMIT = 50;
        const int BACKMODULE_UPPER_LIMIT = 53;
        
        const uint32_t COLOR = 0x70DBDB;
        const double ONE_BRIGHTNESS_STEP = .003;    //speed at which the controller changes the brightness
        const double MAX_BRIGHTNESS = 1.0;
        const double MIN_BRIGHTNESS = 0.0;
        
        Adafruit_DotStar* strip;
        Adafruit_DotStar* stripBack;
        double currentBrightness = 0.0;
    public:
        ColdController(Adafruit_DotStar*, Adafruit_DotStar*);
        void loop();
        void setMode(int mode);
};

ColdController::ColdController(Adafruit_DotStar* injectedStrip, Adafruit_DotStar* injectedStripBack) 
{ 
    strip = injectedStrip; 
    stripBack = injectedStripBack;
}

//Modes:
//0: Off
//1: less cold
//2: not implemented
void ColdController::setMode(int mode)
{
    //off
    if (mode == 0) {
        currentBrightness = MIN_BRIGHTNESS - 1.0; //turn off
    //less
    } else if (mode == 1) {
        currentBrightness -= ONE_BRIGHTNESS_STEP;
        currentBrightness = currentBrightness > MIN_BRIGHTNESS ? currentBrightness : MIN_BRIGHTNESS;
    //more
    } else if (mode == 2) {
        currentBrightness += ONE_BRIGHTNESS_STEP;
        currentBrightness = currentBrightness < MAX_BRIGHTNESS ? currentBrightness : MAX_BRIGHTNESS;
    }
}

void ColdController::loop() {
    //Set color brightness
    CRGB color = COLOR;

    //Turn off
    if (currentBrightness <= MIN_BRIGHTNESS) {
        color = 0x0;
    //Lower brightness
    } else {
        color %= (currentBrightness * 255);
    }

    setStripArrayColor(strip, rgbToHex(color), FRONTMODULE_LOWER_LIMIT, FRONTMODULE_UPPER_LIMIT);
    setStripArrayColor(strip, rgbToHex(color), BACKMODULE_LOWER_LIMIT, BACKMODULE_UPPER_LIMIT);
}
