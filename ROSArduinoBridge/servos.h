#ifndef SERVOS_H
#define SERVOS_H

#include <Servo.h>

// 定义舵机和引脚

#define leftServoPin 2
#define rightServoPin 3

Servo leftServo;
Servo rightServo;

// 舵机控制参数
int g_stepDelayMs = 20; // 每步的延迟（毫秒）

int leftCurrentPositionDegrees = 0;
int leftTargetPositionDegrees = 0;
int leftLastSweepCommand = 0;

int rightCurrentPositionDegrees = 0;
int rightTargetPositionDegrees = 0;
int rightLastSweepCommand = 0;

void initServo(Servo& servo, int servoPin, int initPosition);
void doSweep(Servo& servo, int& currentPosition, int& targetPosition, int& lastCommandTime, int stepDelayMs);
void setTargetPosition(Servo& servo, int position);
int getServoPosition(Servo& servo, int& currentPosition); 

#endif
