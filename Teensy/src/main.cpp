#include "main.hpp"

// DEBUG
#define DEBUG true

// GYRO
#define GYRORESET 3

// ODOMETRY
#define WHEEL_TRACK 201
#define WHEEL_DIAMETER 63.662
#define MIDDLE_ENCODER_RADIUS 25
#define TICKS_PER_REVOLUTION 8192

// ENCODERS
#define LEFT_ENCODER_PORT_A 34
#define LEFT_ENCODER_PORT_B 33
#define LEFT_ENCODER_PORT_X 35

#define MIDDLE_ENCODER_PORT_A 37
#define MIDDLE_ENCODER_PORT_B 36
#define MIDDLE_ENCODER_PORT_X 38

#define RIGHT_ENCODER_PORT_A 40
#define RIGHT_ENCODER_PORT_B 39
#define RIGHT_ENCODER_PORT_X 41

#define FLYWHEEL_ENCODER_PORT_A 15
#define FLYWHEEL_ENCODER_PORT_B 14
#define FLYWHEEL_ENCODER_PORT_X 16

Encoder leftEncoder(LEFT_ENCODER_PORT_A, LEFT_ENCODER_PORT_B);
Encoder middleEncoder(MIDDLE_ENCODER_PORT_A, MIDDLE_ENCODER_PORT_B);
Encoder rightEncoder(RIGHT_ENCODER_PORT_A, RIGHT_ENCODER_PORT_B);
ThreeEncoderOdometry odometry(&leftEncoder, &rightEncoder, &middleEncoder,
                              WHEEL_DIAMETER, MIDDLE_ENCODER_RADIUS,
                              WHEEL_TRACK, TICKS_PER_REVOLUTION);

bool gyroSuccess = true;
bool colourSuccess = true;

SerialHandler serialHandler(115200);
TeensyData teensyData;
V5Data v5Data;

void setupPins() {
    pinMode(GYRORESET, OUTPUT);
    digitalWrite(GYRORESET, HIGH);
    setupPneumatics();
}

void receiveData() {
    while (true) {
        serialHandler.receive();
        v5Data = serialHandler.getV5Data();
        threads.delay(1);
    }
}

void sendData() {
    while (true) {
        serialHandler.send(&teensyData);
        threads.delay(2);
    }
}

void updateGyro() {
    // Gyroscope
    if (gyroSuccess) {
        sensors_event_t event;
        bno.getEvent(&event);
        teensyData.gyroHeading = event.orientation.x;
    }
}

void updateOdom() {
    odometry.update();
    teensyData.heading = odometry.getHeading();
    // Consider Kalman filter
    teensyData.headingMerged = gyroSuccess ? (odometry.getHeading() + teensyData.gyroHeading) / 2 : odometry.getHeading();
    teensyData.x = odometry.getX();
    teensyData.y = odometry.getY();
}

void updateColour() {
    //  Colour Sensor
    if (colourSuccess) {
        uint16_t r, g, b, c;
        tcs.getRawData(&r, &g, &b, &c);
        teensyData.r = (uint8_t)((double)r / c * 255);
        teensyData.g = (uint8_t)((double)g / c * 255);
        teensyData.b = (uint8_t)((double)b / c * 255);
    }
}

void setup() {
    Serial.begin(115200);
    serialHandler.flush();
    setupScreen();
    setupPins();
    gyroSuccess = gyroSetup();
    colourSuccess = colourSetup();
    delay(2000);
    tft.fillScreen(HX8357_BLACK);
    threads.addThread(receiveData);
    threads.addThread(sendData);
    threads.addThread(updatePneumatics);
}

void loop() {
    updateTouch();
    updateGyro();
    updateOdom();
    updateColour();
    Serial.println(v5Data.pistons[1]);
    delay(1);
}