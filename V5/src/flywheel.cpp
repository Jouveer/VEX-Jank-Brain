#include "main.h"

#define MAXSPEED 2000
#define MINSPEED 1000

Motor flywheelTop(3, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::counts);
Motor flywheelBot(4, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::counts);

MotorGroup flywheel({flywheelTop, flywheelBot});
ControllerButton flywheelToggle(ControllerDigital::A);
ControllerButton flywheelPivotToggle(ControllerDigital::X);
ControllerButton speedToggle(ControllerDigital::up);
ControllerButton speedUp(ControllerDigital::right);
ControllerButton speedDown(ControllerDigital::down);
bool flywheelEnabled = false;
bool flywheelPreviouslyDisabled = true;

double targetSpeed = 2600;

void setFlywheelRPM(double targetSpeedIn) {
    targetSpeed = targetSpeedIn;
}
void enableFlywheel(bool enable) {
    flywheelEnabled = enable;
}

void flywheelTask() {
    flywheel.setBrakeMode(AbstractMotor::brakeMode::coast);
    double error = 0;
    double errorSum = 0;
    double errorRateOfChange = 0;
    double previousError = 0;
    double output = 1200;  // Gus works on 1400
    double lastTime = 0;
    bool speedHit = false;
    // double kP = 0.002;
    // double kI = 0;
    // double kD = 0.08;
    double kP = 0.003;
    double kI = 0;
    double kD = 0.0004;
    targetSpeed = 2650;
    while (true) {
        if (flywheelEnabled && !pros::competition::is_disabled()) {
            if (speedUp.changedToReleased() == 1) {
                targetSpeed += 100;
            }
            if (speedDown.changedToReleased() == 1) {
                targetSpeed += -100;
            }

            if (flywheelPreviouslyDisabled && !pros::competition::is_autonomous() && !pros::competition::is_disabled()) {
                // flywheel.moveVoltage(12000);
                // pros::delay(500);
                targetSpeed = 2050;
                output = 1000;
            }
            double controlLoopTime = pros::millis() - lastTime;
            lastTime = pros::millis();
            // flywheel.moveVoltage(7900);
            error = targetSpeed - flywheel.getActualVelocity() * 6;
            errorSum += (error * controlLoopTime);
            errorRateOfChange = (error - previousError) / controlLoopTime;
            previousError = error;
            output += kP * error + kI * errorSum + kD * errorRateOfChange;

            if (error > 50 && !speedHit) {
                flywheel.moveVoltage(12000);
            }

            if (error < 50 && !speedHit) {
                speedHit = true;
            }

            if (error > 30 && (intakeFront.getTargetVelocity() < 0 || controller.getDigital(ControllerDigital::L2))) {
                flywheel.moveVoltage(12000);
            } else if (speedHit) {
                flywheel.moveVoltage(std::clamp(output / 3600 * 12000, -12000.0, 12000.0));
            }
            // flywheel.moveVoltage(12000);

            //     if (flywheel.getActualVelocity() < 500)
            //         flywheel.moveVoltage(9300);
            //     else if (flywheel.getActualVelocity() > 500)
            //         flywheel.moveVoltage(8700);
            flywheelPreviouslyDisabled = false;

        } else {
            flywheel.moveVoltage(0);
            flywheelPreviouslyDisabled = true;
        }
        pros::delay(5);
    }
}

void flywheelControl() {
    if (flywheelToggle.changedToReleased()) flywheelEnabled = !flywheelEnabled;
    if (flywheelPivotToggle.changedToReleased()) setPneumatics(flywheelPneumaticPort, !v5Data.pistons[flywheelPneumaticPort]);
}

pros::Task flywheelFuntionTask(flywheelTask);