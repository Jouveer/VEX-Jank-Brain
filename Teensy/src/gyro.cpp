#include "main.hpp"

Adafruit_BNO055 bno = Adafruit_BNO055(55);

bool gyroSetup() {
    if (!bno.begin()) {
        tft.setTextColor(HX8357_RED, HX8357_BLACK);
        tft.println("BNO055 not Found!");
        return false;
    } else {
        bno.setExtCrystalUse(true);
        tft.setTextColor(HX8357_GREEN, HX8357_BLACK);
        tft.println("BNO055 Connected!");
        return true;
    }
}