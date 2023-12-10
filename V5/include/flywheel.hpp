#pragma once
#include "main.h"

extern Motor flywheelTop;
extern Motor flywheelBot;
extern MotorGroup flywheel;
extern void flywheelControl();

extern void setFlywheelRPM(double targetSpeedIn);
extern void enableFlywheel(bool enable);
// extern void flywheelTask();