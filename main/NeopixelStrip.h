#ifndef NEOPIXEL_STRIP_H
#define NEOPIXEL_STRIP_H

#include "color_lib.h"
#include <FastLED.h>

class NeopixelStrip {
    private:
        CRGB leds[14];
    
    public:
        NeopixelStrip(uint16_t n, uint8_t pin);
        void begin();
        void show();
        void setPixelColor(int index, uint32_t color);
};

NeopixelStrip::NeopixelStrip(uint16_t n, uint8_t pin=3)
{ 
    FastLED.addLeds<NEOPIXEL, 3>(leds, 14);
}

void NeopixelStrip::setPixelColor(int index, uint32_t color)
{
    leds[index] = color;
}

void NeopixelStrip::show()
{
    FastLED.show();
}

void NeopixelStrip::begin()
{
    
}

#endif /* NEOPIXEL_STRIP_H */
