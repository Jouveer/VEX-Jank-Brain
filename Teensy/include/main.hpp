#pragma once
#include <Adafruit_BNO055.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Encoder.h>
#include <SPI.h>
#include <math.h>
#include <utility/imumaths.h>

#include "Adafruit_HX8357.h"
#include "Adafruit_TCS34725.h"
#include "Adafruit_TSC2007.h"
#include "TeensyThreads.h"
#include "TouchScreen.h"
#include "V5/cobs.hpp"
#include "V5/serialHandler.hpp"
#include "colour.hpp"
#include "gyro.hpp"
#include "odometry.hpp"
#include "pneumatics.hpp"
#include "screen.hpp"

extern V5Data v5Data;