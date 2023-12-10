#include "main.h"

#include <stdarg.h>
#include <string.h>

SerialHandler serialHandler(1, 2, 115200);
TeensyData teensyData;
V5Data v5Data;

double gyroOffset = 0;
bool timer = true;
bool debug = false;

bool autonKilled = false;

uint32_t timerVal = 0;

void receiveTeensy() {
    while (true) {
        serialHandler.receive();
        teensyData = serialHandler.getTeensyData();
        pros::delay(1);
    }
}

void sendTeensy() {
    while (true) {
        serialHandler.send(&v5Data);
        pros::delay(15);
    }
}

void lcdDebug() {
    while (true) {
        pros::lcd::print(0, "X: %d", teensyData.x);
        pros::lcd::print(1, "Y: %d", teensyData.y);
        if (!timer)
            pros::lcd::print(2, "Heading: %f", teensyData.heading);
        else
            pros::lcd::print(2, "Timer: %d", timerVal);
        pros::lcd::print(3, "Gyro: %f", teensyData.gyroHeading - gyroOffset);
        pros::lcd::print(4, "Merged Heading: %f", teensyData.headingMerged);
        pros::lcd::print(5, "R: %d", teensyData.r);
        pros::lcd::print(6, "G: %d", teensyData.g);
        pros::lcd::print(7, "B: %d", teensyData.b);
        delay(10);
    }
}

void gyroLabel() {
    while (true) {
        lv_label_set_text(labelGyro,
                          std::to_string(teensyData.gyroHeading).c_str());
        delay(10);
    }
}

void autonKiller() {
    int32_t startTime = 0;
    while (!autonKilled) {
        if (teensyData.gyroHeading == 0.0 &&
            pros::competition::is_autonomous() &&
            !pros::competition::is_disabled()) {
            startTime = pros::millis();
            while (teensyData.gyroHeading == 0.0) {
                if ((pros::millis() - startTime) > 4000) {
                    autonKilled = true;
                }
                delay(50);
            }
        }
        delay(50);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
    serialHandler.flush();
    v5Data.disabled = true;
    for (int port = 0; port < 6; port++) setPneumatics(port, false);
    pros::Task receiveTeensyTask(receiveTeensy);
    pros::Task sendTeensyTask(sendTeensy);
    pros::Task autonKillerTask(autonKiller);
    if (debug) {
        pros::lcd::initialize();
        pros::Task debugTask(lcdDebug);
    } else {
        pros::Task gyroLabelTask(gyroLabel);
    }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() { v5Data.disabled = true; }

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
    if (!debug) {
        drawAutonSelector();
    }
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
    int32_t startTime = pros::millis();
    chassisModel->setBrakeMode(AbstractMotor::brakeMode::brake);
    v5Data.disabled = false;
    gyroOffset = teensyData.gyroHeading;
    if (autonEnabled) {
        if (autonGustavo) {
            if (autonSimple) {
                basicGus();
            } else {
                discRushGus();
            }
        } else {
            if (autonSimple) {
                basicSus();
            } else {
                // OH NO!
                basicSus();
            }
        }
    }
    timerVal = pros::millis() - startTime;
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    v5Data.disabled = false;
    chassisModel->setBrakeMode(AbstractMotor::brakeMode::coast);
    setFlywheelRPM(2050);
    enableFlywheel(true);
    setPneumatics(flywheelPneumaticPort, false);
    while (true) {
        driveControl();
        rollerControl();
        intakeControl();
        flywheelControl();
        expansionControl();
        pros::delay(5);
    }
}
