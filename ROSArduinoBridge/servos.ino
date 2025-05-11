
#ifdef USE_SERVOS
#include "servos.h"

void initServo(Servo& servo, int servoPin, int initPosition) {
    servo.attach(servoPin, 500, 2500);          
    servo.write(initPosition);      
}

void doSweep(Servo& servo, int& currentPosition, int& targetPosition, int& lastCommandTime, int stepDelayMs) {
    int delta = millis() - lastCommandTime; // 计算时间差
    if (delta > stepDelayMs) {
        if (targetPosition > currentPosition) {
            currentPosition++;        // 增加角度
            servo.write(currentPosition);
        } else if (targetPosition < currentPosition) {
            currentPosition--;        // 减小角度
            servo.write(currentPosition);
        }
        lastCommandTime = millis();   // 更新时间
    }
}


// Set a new target position
void setTargetPosition(Servo& servo, int position) {
    position = constrain(position, 0, 180); // 限制角度范围在 0 到 180
    if (position <= 40){
      position=40;
    }else if (position >= 150){
      position=150;
    }
    servo.write(position);         
}

int getServoPosition(Servo& servo, int& currentPosition) {
    return currentPosition; // 返回当前存储的角度值
}
#endif
