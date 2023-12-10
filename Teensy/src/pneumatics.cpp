#include "main.hpp"

#define S1C 32
#define S2C 31
#define S3C 30
#define S4C 29
#define S5C 28
#define S6C 27

uint8_t pistons[6] = {S1C, S2C, S3C, S4C, S5C, S6C};

void setupPneumatics() {
    pinMode(S1C, OUTPUT);
    pinMode(S2C, OUTPUT);
    pinMode(S3C, OUTPUT);
    pinMode(S4C, OUTPUT);
    pinMode(S5C, OUTPUT);
    pinMode(S6C, OUTPUT);
}

void updatePneumatics() {
    while (true) {
        if (!v5Data.disabled) {
            digitalWrite(S1C, v5Data.pistons[0]);
            digitalWrite(S2C, v5Data.pistons[1]);
            digitalWrite(S3C, v5Data.pistons[2]);
            digitalWrite(S4C, v5Data.pistons[3]);
            digitalWrite(S5C, v5Data.pistons[4]);
            digitalWrite(S6C, v5Data.pistons[5]);
        }
        threads.delay(5);
    }
}