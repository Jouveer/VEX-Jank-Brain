#include "main.h"

ControllerButton expansionToggle1(ControllerDigital::left);
ControllerButton expansionToggle2(ControllerDigital::Y);

void expansionControl() {
    if (expansionToggle1.isPressed() && expansionToggle2.isPressed())
        setPneumatics(expansionPneumaticPort, true);
}