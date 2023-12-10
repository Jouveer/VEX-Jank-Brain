#include "main.hpp"

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 250
#define TS_MINY 230
#define TS_MAXX 3750
#define TS_MAXY 2050

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// The display uses hardware SPI, plus #9 & #10
#define TFT_RST -1  // dont use a reset pin, tie to arduino RST if you like
#define TFT_DC 9
#define TFT_CS 10

#define DEBUG false

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
Adafruit_TSC2007 touch;
TSPoint point;
uint16_t x, y, z1, z2;

void setupScreen() {
    tft.begin();
    tft.fillScreen(HX8357_BLACK);
    tft.setTextSize(2);
    tft.setRotation(1);
    touch.begin();
}

void updateTouch() {
    point.z = 0;
    if (touch.read_touch(&x, &y, &z1, &z2)) {
        point.x = x;
        point.y = y;
        point.z = z1;
    }
    point.x = map(point.x, TS_MINX, TS_MAXX, 0, tft.width());
    point.y = map(point.y, TS_MINY, TS_MAXY, 0, tft.height());
    if (point.z >= 100 && DEBUG) {
        Serial.print("X: ");
        Serial.println(point.x);
        Serial.print("Y: ");
        Serial.println(point.y);
        Serial.print("Z: ");
        Serial.println(point.z);
        Serial.println();
    }
}

TSPoint getPoint() { return point; }

void printToScreen(double x, double y, double heading, double leftEncoder,
                   double rightEncoder, double centerEncoder) {
    tft.setTextColor(HX8357_RED, HX8357_BLACK);
    tft.setCursor(0, 0);
    tft.print("X: ");
    tft.println(x);
    tft.print("Y: ");
    tft.println(y);
    tft.print("Heading: ");
    tft.println(heading);
    tft.print("Left Encoder: ");
    tft.println(leftEncoder);
    tft.print("Right Encoder: ");
    tft.println(rightEncoder);
    tft.print("Center Encoder: ");
    tft.println(centerEncoder);
}