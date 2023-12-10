#include "drive.hpp"

#include "main.h"

#define DEADZONE 0.05

ControllerButton switch1(ControllerDigital::R1);
ControllerButton switch2(ControllerDigital::R2);
ControllerButton switch3(ControllerDigital::L1);
ControllerButton switch4(ControllerDigital::L2);

bool arcadeDrive = false;
bool driveLatch = false;

Motor leftDriveBack(11, true, AbstractMotor::gearset::blue,
                    AbstractMotor::encoderUnits::counts);
Motor leftDriveMiddleTop(12, false, AbstractMotor::gearset::blue,
                         AbstractMotor::encoderUnits::counts);
Motor leftDriveMiddleBottom(13, true, AbstractMotor::gearset::blue,
                            AbstractMotor::encoderUnits::counts);
Motor leftDriveFront(14, true, AbstractMotor::gearset::blue,
                     AbstractMotor::encoderUnits::counts);

Motor rightDriveBack(16, false, AbstractMotor::gearset::blue,
                     AbstractMotor::encoderUnits::counts);
Motor rightDriveMiddleTop(17, true, AbstractMotor::gearset::blue,
                          AbstractMotor::encoderUnits::counts);
Motor rightDriveMiddleBottom(18, false, AbstractMotor::gearset::blue,
                             AbstractMotor::encoderUnits::counts);
Motor rightDriveFront(20, false, AbstractMotor::gearset::blue,
                      AbstractMotor::encoderUnits::counts);

MotorGroup leftDrive({leftDriveBack, leftDriveMiddleTop, leftDriveMiddleBottom,
                      leftDriveFront});
MotorGroup rightDrive({rightDriveBack, rightDriveMiddleTop,
                       rightDriveMiddleBottom, rightDriveFront});

std::shared_ptr<okapi::OdomChassisController> chassisController =
    ChassisControllerBuilder()
        .withMotors(leftDrive, rightDrive)
        .withDimensions({AbstractMotor::gearset::blue, 24.0 / 35.0},
                        {{3.125_in, 32_cm}, imev5BlueTPR})
        .withOdometry()
        .buildOdometry();

std::shared_ptr<okapi::ChassisModel> chassisModel =
    chassisController->getModel();

void driveControl() {
    if (switch1.isPressed() && switch2.isPressed() && switch3.isPressed() &&
        switch4.isPressed()) {
        if (!driveLatch) {
            driveLatch = true;
            arcadeDrive = !arcadeDrive;
            controller.rumble("...");
        }
    } else {
        driveLatch = false;
    }

    if (arcadeDrive) {
        chassisModel->arcade(controller.getAnalog(ControllerAnalog::leftY),
                             controller.getAnalog(ControllerAnalog::rightX),
                             0.05);
    } else {
        chassisModel->tank(controller.getAnalog(ControllerAnalog::leftY),
                           controller.getAnalog(ControllerAnalog::rightY), 0.05);
    }
}

double constrainAngle(double angle) {
    return atan2(sin(angle * degreeToRadian), cos(angle * degreeToRadian)) *
           radianToDegree;
}

void gyroTurn(double angle, double kP, double kI, double kD, int timeout) {
    double error;
    double lastError = 0;
    double derivative = 0;
    double integral = 0;
    double speed;
    int targerCounter = 0;
    const double maxIntegral = 200;
    const double minIntegral = -maxIntegral;
    int32_t startTime = pros::millis();

    while (targerCounter <= 1000) {
        if ((pros::millis() - startTime) > timeout) break;
        error = constrainAngle((teensyData.gyroHeading - gyroOffset) - angle);
        derivative = error - lastError;
        lastError = error;
        integral += error;
        integral = std::clamp(integral, minIntegral, maxIntegral);

        speed = (error * kP) + (integral * kI) + (derivative * kD);
        chassisModel->rotate(-speed / 360);
        targerCounter = fabs(error) <= 0.25 ? targerCounter + 1 : 0;
    }
    chassisModel->stop();
}

void gyroDrive(double angle, double kP, double kD, double drivekP,
               double drivekD, double distance, double minSpeed,
               double maxSpeed, bool reverse) {
    double error;
    double lastError = 0;
    double derivative = 0;
    double turnSpeed;

    double driveError;
    double driveLastError = 0;
    double driveDerivative = 0;
    double driveSpeed;

    double wheelSpeedLeft;
    double wheelSpeedRight;

    double currentDistance = 0;
    const double wheelCircumference = 3.25 * inchToMM / 10 * pi;
    const double ticksPerCM = (imev5BlueTPR * 35.0 / 24.0) / wheelCircumference;

    chassisModel->resetSensors();

    while (currentDistance < distance * ticksPerCM) {

        if(autonKilled){
            while(true){
                chassisModel->stop();
                delay(50);
            }
        }

        std::cout << "Gyro: " << teensyData.gyroHeading << std::endl;
        currentDistance = (abs(chassisModel->getSensorVals()[0]) +
                           abs(chassisModel->getSensorVals()[1])) /
                          2;

        error = constrainAngle((teensyData.gyroHeading - gyroOffset) - angle);
        derivative = error - lastError;
        lastError = error;

        turnSpeed = (error * kP) + (derivative * kD);

        driveError = distance - currentDistance;
        driveDerivative = driveError - driveLastError;
        driveLastError = driveError;

        driveSpeed = (driveError * drivekP) + (driveDerivative * drivekD);
        driveSpeed = std::clamp(driveSpeed, minSpeed, maxSpeed);
        driveSpeed = reverse ? -driveSpeed : driveSpeed;

        wheelSpeedLeft = driveSpeed - turnSpeed;
        wheelSpeedRight = driveSpeed + turnSpeed;

        leftDrive.moveVoltage(wheelSpeedLeft / 600 * 12000);
        rightDrive.moveVoltage(wheelSpeedRight / 600 * 12000);
    }
    chassisModel->stop();
}

void gyroDrive(double angle, std::vector<std::pair<double, double>> angleArray,
               double kP, double kD, double drivekP, double drivekD,
               double distance, double minSpeed, double maxSpeed,
               bool reverse) {
    double targetAngle = angle;

    double error;
    double lastError = 0;
    double derivative = 0;
    double turnSpeed;

    double driveError;
    double driveLastError = 0;
    double driveDerivative = 0;
    double driveSpeed;

    double wheelSpeedLeft;
    double wheelSpeedRight;

    double currentDistance;
    const double wheelCircumference = 3.25 * inchToMM / 10 * pi;
    const double ticksPerCM = (imev5BlueTPR * 35.0 / 24.0) / wheelCircumference;

    int currentPos = 0;

    chassisModel->resetSensors();

    while (currentDistance < distance * ticksPerCM) {
        currentDistance = (abs(chassisModel->getSensorVals()[0]) +
                           abs(chassisModel->getSensorVals()[1])) /
                          2;

        if (currentDistance / ticksPerCM > angleArray[currentPos].second) {
            if (currentPos < angleArray.size() - 1) currentPos++;
            targetAngle = angleArray[currentPos].first;
        }

        error =
            constrainAngle((teensyData.gyroHeading - gyroOffset) - targetAngle);
        derivative = error - lastError;
        lastError = error;

        turnSpeed = (error * kP) + (derivative * kD);

        driveError = distance * ticksPerCM - currentDistance;
        driveDerivative = driveError - driveLastError;
        driveLastError = driveError;

        driveSpeed = (driveError * drivekP) + (driveDerivative * drivekD);
        driveSpeed = std::clamp(driveSpeed, minSpeed, maxSpeed);
        driveSpeed = reverse ? -driveSpeed : driveSpeed;

        wheelSpeedLeft = driveSpeed - turnSpeed;
        wheelSpeedRight = driveSpeed + turnSpeed;

        leftDrive.moveVoltage(wheelSpeedLeft / 600 * 12000);
        rightDrive.moveVoltage(wheelSpeedRight / 600 * 12000);
    }
    chassisModel->stop();
}
