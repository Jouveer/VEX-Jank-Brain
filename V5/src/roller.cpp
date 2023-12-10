#include "main.h"

#define colourLimit 80

Motor roller(7);
int rollerSpeed;
ControllerButton rollerToggleButton(ControllerDigital::B);
bool rollerToggle = false;

void rollerControl() {
    roller.setBrakeMode(AbstractMotor::brakeMode::brake);
    rollerSpeed = controller.getDigital(ControllerDigital::R1)   ? 200
                  : controller.getDigital(ControllerDigital::R2) ? -200
                                                                 : 0;
    turnToColourToggle();
}

void turnToColour(bool red, int timeout) {
    roller.setBrakeMode(AbstractMotor::brakeMode::brake);
    int colourValue = 0;
    int32_t startTimeRoller = pros::millis();
    while (colourValue <= colourLimit) {
        if((pros::millis() - startTimeRoller) > timeout){
            break;
        }
        colourValue = red ? teensyData.r : teensyData.b;
        roller.moveVelocity(-160);
        pros::delay(5);
    }
    roller.moveVoltage(0);
}

void turnToColourToggle() {
    if (rollerToggleButton.changedToReleased()) {
        rollerToggle = !rollerToggle;
        if (rollerToggle) controller.rumble("-");
    }
    int colourValue = autonRed ? teensyData.r : teensyData.b;
    if (rollerToggle && colourValue <= colourLimit &&
        !(controller.getDigital(ControllerDigital::R1) ||
          controller.getDigital(ControllerDigital::R2))) {
        roller.moveVelocity(-120);
    } else {
        roller.moveVelocity(rollerSpeed);
    }
}