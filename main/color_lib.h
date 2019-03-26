#ifndef COLOR_LIB_H
#define COLOR_LIB_H

#include <FastLED.h>
#include <Adafruit_DotStar.h>

// uint32_t linearShade(float percent, uint32_t color) {
//         int r = ((color >> 16) & 0xFF); // Extract the RR byte
//         int b = ((color >> 8)  & 0xFF); // Extract the GG byte
//         int g = ((color)       & 0xFF); // Extract the BB byte

//         bool isPositive = percent < 0;
//         float t = isPositive ? 0 : 255 * percent;
//         float P = isPositive ? 1 + percent : 1 - percent;

//         r = r * P + t;
//         b = b * P + t;
//         g = g * P + t;
//     return strip.Color(r, b, g);
// }

void setStripArrayColor(Adafruit_DotStar* strip, uint32_t color, int lowerIndex, int upperIndex) {
    for (int i = lowerIndex; i <= upperIndex; i++) {
        strip->setPixelColor(i, color);
    }
}

bool counter(int &counter, int timeout)
{
    counter++;
    if (counter > timeout) {
        counter = 0;
        return true;
    } else {
        return false;
    }
}

uint32_t rgbToHex(CRGB color)
{
    uint32_t r = color.r;
    uint32_t b = color.b;
    uint32_t g = color.g;
    return ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF);
}

CHSV rgbToHsv(CRGB in)
{
    CHSV        out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = 0.0;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}

#endif /* COLOR_LIB_H */