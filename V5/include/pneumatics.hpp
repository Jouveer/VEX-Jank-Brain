#pragma once
#include "main.h"

#define flywheelPneumaticPort 0   // J1 Flywheel
#define intakePneumaticPort 2     // J3 Intake
#define expansionPneumaticPort 4  // J5 Expansion

extern void setPneumatics(int port, bool value);