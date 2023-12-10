#include "main.h"

Motor intakeRear(5, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::counts);
Motor intakeFront(6, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::counts);
MotorGroup intake({intakeRear, intakeFront});
ControllerButton intakePivotToggle(ControllerDigital::up);
int intakeSpeed = 0;

void intakeControl() {
    intakeSpeed = controller.getDigital(ControllerDigital::L1)   ? 600
                  : controller.getDigital(ControllerDigital::L2) ? -60
                                                                 : 0;
    intakeRear.moveVelocity(intakeSpeed);
    intakeFront.moveVelocity(intakeSpeed);
    if (intakePivotToggle.changedToReleased()) setPneumatics(intakePneumaticPort, !v5Data.pistons[intakePneumaticPort]);
}