#pragma once
#include "Adafruit_HX8357.h"

extern Adafruit_HX8357 tft;
extern void setupScreen();
extern void updateTouch();
extern void printToScreen(double x, double y, double heading,
                          double leftEncoder, double rightEncoder,
                          double centerEncoder);