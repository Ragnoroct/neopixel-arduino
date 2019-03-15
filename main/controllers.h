#include <Adafruit_DotStar.h>
#define NUMPIXELS 60 // Number of LEDs in strip

// Health functions
class HealthController {
    private:
        Adafruit_DotStar* strip;
    public:
        HealthController(Adafruit_DotStar*);
        void loop();
};

HealthController::HealthController(Adafruit_DotStar* strip) {
    this->strip = strip;
}


int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

void HealthController::loop() {
    strip->setPixelColor(head, color); // 'On' pixel at head
    strip->setPixelColor(tail, 0);     // 'Off' pixel at tail
    strip->show();                     // Refresh strip
    delay(5);                        // Pause 20 milliseconds (~50 FPS)

    if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
        head = 0;                       //  Yes, reset head index to start
        if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
        color = 0xFF0000;             //   Yes, reset to red
    }
    if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
} 