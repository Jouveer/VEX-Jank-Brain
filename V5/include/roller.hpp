#pragma once
#include "main.h"

extern void rollerControl();
extern void turnToColour(bool red, int timeout = 2000);
extern void turnToColourToggle();
extern Motor roller;