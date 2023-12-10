#include "main.h"

void setPneumatics(int port, bool value) {
    v5Data.pistons[port] = value;
}