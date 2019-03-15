#include <Adafruit_DotStar.h>

class HealthController {
    private:
        Adafruit_DotStar* strip;
        const int HEALTH_RED = 0xE80000;
        const int FRONT = 7;
        const int BACK = 0;
        int currentPixel = 0;
    public:
        HealthController(Adafruit_DotStar*);
        void loop();
};

#define NUMPIXELS 60 // Number of LEDs in strip
#define DATAPIN    1
#define CLOCKPIN   0

// Adafruit_DotStar strip = 

#define NUMPIXELS 60 // Number of LEDs in strip
int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

HealthController::HealthController(Adafruit_DotStar* injectedStrip)
{
    strip = new Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
}

void HealthController::loop() {
    Serial.println("Ugh");
    strip->setPixelColor(10, 0xFF0000); // 'On' pixel at head
    // strip->setPixelColor(tail, 0);     // 'Off' pixel at tail
    strip->show();                     // Refresh strip
    delay(5);                        // Pause 20 milliseconds (~50 FPS)

    if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
        head = 0;                       //  Yes, reset head index to start
        if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
        color = 0xFF0000;             //   Yes, reset to red
    }
    if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
} 