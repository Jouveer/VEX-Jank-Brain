#pragma once
#include "main.h"

extern std::shared_ptr<okapi::ChassisModel> chassisModel;
extern std::shared_ptr<okapi::OdomChassisController> chassisController;
extern MotorGroup leftDrive;
extern MotorGroup rightDrive;
extern bool autonKilled;

extern void driveControl();
extern double gyroOffset;
void gyroTurn(double angle, double kP, double kI, double kD, int timeout = 4000);
extern void gyroDrive(double angle, double kP, double kD, double drivekP,
                      double drivekD, double distance, double minSpeed,
                      double maxSpeed, bool reverse = false);
extern void gyroDrive(double angle,
                      std::vector<std::pair<double, double>> angleArray,
                      double kP, double kD, double drivekP, double drivekD,
                      double distance, double minSpeed, double maxSpeed,
                      bool reverse = false);