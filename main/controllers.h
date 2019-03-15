#include <Adafruit_DotStar.h>

namespace Controllers
{
    // Health functions
    class HealthController {
        private:
            Adafruit_DotStar* strip2;
            int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
            uint32_t color = 0xFF0000;      // 'On' color (starts red)
            const int HEALTH_RED = 0xE80000;
            const int FRONT = 7;
            const int BACK = 0;
            int currentPixel = 0;
        public:
            HealthController(Adafruit_DotStar*);
            void loop();
    };

    HealthController::HealthController(Adafruit_DotStar* strip) {
        this->strip2 = strip;
    }

// Adafruit_DotStar strip = 

    void HealthController::loop() {
        int tail;

        // if (currentPixel == BACK)
        //     tail = FRONT;
        // else if (currentPixel == FRONT)
        //     tail = BACK;
        // else
        //     tail = currentPixel - 1;

        this->strip2->setPixelColor(30, 0xFF0000);

        // strip->setPixelColor(tail, 0); // 'On' pixel at head
        // strip->setPixelColor(currentPixel, HEALTH_RED);     // 'Off' pixel at tail
        this->strip2->show();                     // Refresh strip
        delay(500);                        // Pause 20 milliseconds (~50 FPS)

        // Serial.println(currentPixel);

        // currentPixel++;

        // if (currentPixel > FRONT) {
        //     currentPixel = BACK;
        // }
    } 
}

// void HealthController::loop() {
//     Serial.println("Ugh");
//     strip->setPixelColor(10, 0xFF0000); // 'On' pixel at head
//     // strip->setPixelColor(tail, 0);     // 'Off' pixel at tail
//     strip->show();                     // Refresh strip
//     delay(5);                        // Pause 20 milliseconds (~50 FPS)

//     if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
//         head = 0;                       //  Yes, reset head index to start
//         if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
//         color = 0xFF0000;             //   Yes, reset to red
//     }
//     if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
// } 