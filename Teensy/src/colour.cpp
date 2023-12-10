#include "main.hpp"

// LIGHT
#define LIGHTPIN 2

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_1X);

bool colourSetup() {
    if (!tcs.begin((uint8_t)41U, &Wire2)) {
        tft.setTextColor(HX8357_RED, HX8357_BLACK);
        tft.println("TCS34725 not Found!");
        return false;
    } else {
        tft.setTextColor(HX8357_GREEN, HX8357_BLACK);
        tft.println("TCS34725 Connected!");
        pinMode(LIGHTPIN, OUTPUT_OPENDRAIN);
        digitalWrite(LIGHTPIN, HIGH);
        return true;
    }
}