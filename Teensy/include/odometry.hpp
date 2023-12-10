#pragma once
#include <Encoder.h>
class ThreeEncoderOdometry {
   public:
    ThreeEncoderOdometry(Encoder *leftEncoder, Encoder *rightEncoder,
                         Encoder *centerEncoder, float wheelDiameter,
                         float wheelTrack, float mRadius, int TPR);
    void update();
    void reset();
    void setWheelDiameter(float diameter);
    void setWheelBase(float base);
    void setTicksPerRevolution(int ticks);
    void setLeftEncoder(int ticks);
    void setRightEncoder(int ticks);
    void setCenterEncoder(int ticks);
    long getLeftEncoder();
    long getRightEncoder();
    long getCenterEncoder();
    float getX();
    float getY();
    float getHeading();
    const float maxTickDiff = 15000;

   private:
    Encoder *leftEncoder;
    Encoder *rightEncoder;
    Encoder *centerEncoder;
    float wheelDiameter;
    float wheelTrack;
    float mRadius;
    int TPR;
    long leftEncoderTicks;
    long rightEncoderTicks;
    long centerEncoderTicks;
    double mmPerTick;
    float x = 0;
    float y = 0;
    float heading = 0;
};