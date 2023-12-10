#include "main.hpp"

ThreeEncoderOdometry::ThreeEncoderOdometry(Encoder *leftEncoder,
                                           Encoder *rightEncoder,
                                           Encoder *centerEncoder,
                                           float wheelDiameter, float mRadius,
                                           float wheelTrack, int TPR)
    : leftEncoder(leftEncoder),
      rightEncoder(rightEncoder),
      centerEncoder(centerEncoder),
      wheelDiameter(wheelDiameter),
      wheelTrack(wheelTrack),
      mRadius(mRadius),
      TPR(TPR) {
    mmPerTick = (wheelDiameter * M_PI) / TPR;
    reset();
}

void ThreeEncoderOdometry::reset() {
    leftEncoderTicks = 0;
    rightEncoderTicks = 0;
    centerEncoderTicks = 0;
    x = 0;
    y = 0;
    heading = 0;
    leftEncoder->write(0);
    rightEncoder->write(0);
    centerEncoder->write(0);
}

void ThreeEncoderOdometry::setWheelDiameter(float diameter) {
    wheelDiameter = diameter;
}

void ThreeEncoderOdometry::setWheelBase(float base) { wheelTrack = base; }

void ThreeEncoderOdometry::setTicksPerRevolution(int ticks) { TPR = ticks; }

void ThreeEncoderOdometry::setLeftEncoder(int ticks) {
    leftEncoderTicks = ticks;
}

void ThreeEncoderOdometry::setRightEncoder(int ticks) {
    rightEncoderTicks = ticks;
}

void ThreeEncoderOdometry::setCenterEncoder(int ticks) {
    centerEncoderTicks = ticks;
}

long ThreeEncoderOdometry::getLeftEncoder() { return leftEncoderTicks; }
long ThreeEncoderOdometry::getRightEncoder() { return rightEncoderTicks; }
long ThreeEncoderOdometry::getCenterEncoder() { return centerEncoderTicks; }

float ThreeEncoderOdometry::getX() { return x; }
float ThreeEncoderOdometry::getY() { return y; }
float ThreeEncoderOdometry::getHeading() { return heading * RAD_TO_DEG; }

double constrainAngle(double angle) { return atan2(sin(angle), cos(angle)); }

void ThreeEncoderOdometry::update() {
    // Read the encoder values
    long lTickNew = leftEncoder->read();
    long rTickNew = -rightEncoder->read();
    long cTickNew = centerEncoder->read();

    // Calculate the change in encoder ticks
    float lDelta = lTickNew - leftEncoderTicks;
    float rDelta = rTickNew - rightEncoderTicks;
    float cDelta = cTickNew - centerEncoderTicks;

    // Update previous encoder ticks
    leftEncoderTicks = lTickNew;
    rightEncoderTicks = rTickNew;
    centerEncoderTicks = cTickNew;

    if (fabs(lDelta) > maxTickDiff || fabs(rDelta) > maxTickDiff ||
        fabs(cDelta) > maxTickDiff) {
        Serial.println("Encoder tick difference too large, skipping update");
        return;
    }

    double dtheta = mmPerTick * (rDelta - lDelta) / wheelTrack;
    double dx = mmPerTick * (rDelta + lDelta) / 2.0;
    double dy = mmPerTick * (cDelta - (rDelta - lDelta) * mRadius / wheelTrack);

    double theta = heading + dtheta / 2.0;
    x += dx * cos(theta) - dy * sin(theta);
    y += dx * sin(theta) + dy * cos(theta);
    heading += dtheta;

    // Keep heading between 0 and 2pi
    heading = constrainAngle(heading);
}
