#define USE_BASE        // 启用底盘控制

#ifdef USE_BASE
  #define ARDUINO_MY_COUNTER
  #define TB6612_MOTOR_DRIVER
#endif

#define USE_SERVOS        // 启用舵机

#define BAUDRATE 57600
#define MAX_PWM 255

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "commands.h"
// #include "sensors.h"

#ifdef USE_SERVOS
   #include <Servo.h>
   #include "servos.h"
#endif

//---------------------------------------------------------------------------------------------------start
#ifdef USE_BASE
  #include "motor_driver.h"         //电机驱动
  #include "encoder_driver.h"       //编码器驱动
  #include "diff_controller.h"      //PID调速
  #define PID_RATE 30               // Hz
  const int PID_INTERVAL = 1000 / PID_RATE;
  unsigned long nextPID = PID_INTERVAL;
  #define AUTO_STOP_INTERVAL 10000
  long lastMotorCommand = AUTO_STOP_INTERVAL;
#endif
//---------------------------------------------------------------------------------------------------end

int arg = 0;
int index = 0;
char chr;
char cmd;
char argv1[16];
char argv2[16];
long arg1;
long arg2;
int leftPos = 90;  // 舵机初始位置
int rightPos = 90;

void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  index = 0;
}

int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  
  switch(cmd) {
  case GET_BAUDRATE:
    Serial.println(BAUDRATE);
    break;
  case ANALOG_READ:
    Serial.println(analogRead(arg1));
    break;
  case DIGITAL_READ:
    Serial.println(digitalRead(arg1));
    break;
  case ANALOG_WRITE:
    analogWrite(arg1, arg2);
    Serial.println("OK"); 
    break;
  case DIGITAL_WRITE:
    if (arg2 == 0) digitalWrite(arg1, LOW);
    else if (arg2 == 1) digitalWrite(arg1, HIGH);
    Serial.println("OK"); 
    break;
  case PIN_MODE:
    if (arg2 == 0) pinMode(arg1, INPUT);
    else if (arg2 == 1) pinMode(arg1, OUTPUT);
    Serial.println("OK");
    break;
  // case PING:
  //   Serial.println(Ping(arg1));
  //   break;

//--------------------舵机------------------------
#ifdef USE_SERVOS
  case SERVO_WRITE:
    if (arg1 >= 0 && arg1 <=180) { // 左舵机
      leftServo.write(arg1);  // 设置左舵机的角度
      Serial.println("Left Servo Position Set");
      Serial.print("arg1:");
      Serial.println(sizeof(arg1));
    }
    if (arg2 >= 0 && arg2 <=180) { // 右舵机
      rightServo.write(arg2);  // 设置右舵机的角度
      Serial.println("Right Servo Position Set");
    }
    break;

  case SERVO_READ:
    // 获取舵机角度
    if (arg1 == 0 && arg2 == 1) { // 左舵机
      Serial.print("Left Servo Angle: ");
      Serial.println(leftServo.read());  
      Serial.print("Right Servo Angle: ");
      Serial.println(rightServo.read());  
    }
    break;
#endif
    
#ifdef USE_BASE
  case READ_ENCODERS:
    Serial.print(readEncoder(LEFT));
    Serial.print(" ");
    Serial.println(readEncoder(RIGHT));
    break;
   case RESET_ENCODERS:
    resetEncoders();
    resetPID();
    Serial.println("OK");
    break;

  case MOTOR_SPEEDS:
    lastMotorCommand = millis();
    if (arg1 == 0 && arg2 == 0) {
      setMotorSpeeds(0, 0);
      resetPID();
      moving = 0;
    }
    else moving = 1;
    //设置PID调试的目标值
    leftPID.TargetTicksPerFrame = arg1;
    rightPID.TargetTicksPerFrame = arg2;
    Serial.println("OK"); 
    break;
    
  case UPDATE_PID:
    while ((str = strtok_r(p, ":", &p)) != '\0') {
       pid_args[i] = atoi(str);
       i++;
    }
    Kp = pid_args[0];
    Kd = pid_args[1];
    Ki = pid_args[2];
    Ko = pid_args[3];
    Serial.println("OK");
    break;
#endif
  default:
    Serial.println("Invalid Command");
    break;
  }
}

/* Setup function--runs once at startup. */
void setup() {
  Serial.begin(BAUDRATE);
  #ifdef USE_BASE
    #ifdef ARDUINO_MY_COUNTER
      initEncoder();
      
    #endif
    moving = 1;
    initMotorController();
    resetPID();
    Serial.println("OK"); 
  #endif

  #ifdef USE_SERVOS
    initServo(leftServo, leftServoPin, 90);
    initServo(rightServo, rightServoPin, 90);
  #endif
}

  
void loop() {
  // while (1) {
  // // leftServo.write(45);
  // setMotorSpeeds(200, 200); 
  // }
  static bool hasTurned = false;
  if (!hasTurned) {
    // 右转（左轮正转，右轮反转）
    setMotorSpeeds(3000, 3000);
    delay(3000);
    leftServo.write(45);
    rightServo.write(90);
    setMotorSpeeds(3000, 3000);
    delay(2000);  // 持续1秒
    
    leftServo.write(90);
    rightServo.write(90);
    setMotorSpeeds(3000, 3000);
    delay(5000);  // 持续1秒
    setMotorSpeeds(0, 0);
    hasTurned = true;
  }

  while (Serial.available() > 0) {
    
    chr = Serial.read();

    if (chr == 13) {
      if (arg == 1) argv1[index] = NULL;
      else if (arg == 2) argv2[index] = NULL;
      runCommand();
      resetCommand();
    }
    // Use spaces to delimit parts of the command
    else if (chr == ' ') {
      // Step through the arguments
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index] = NULL;
        arg = 2;
        index = 0;
      }
      continue;
    }
    else {
      if (arg == 0) {
        // The first arg is the single-letter command
        cmd = chr;
      }
      else if (arg == 1) {
        // Subsequent arguments can be more than one character
        argv1[index] = chr;
        index++;
      }
      else if (arg == 2) {
        argv2[index] = chr;
        index++;
      }
    }
  }
  

//---------------------------------------------------------------------------------------------------start
#ifdef USE_BASE
  if (millis() > nextPID) {//循环执行PID调试
    updatePID();
    nextPID += PID_INTERVAL;
  }
  
  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {;
    setMotorSpeeds(0, 0);
    moving = 0;
  }
#endif
//---------------------------------------------------------------------------------------------------end

// Sweep servos
#ifdef USE_SERVOS
// 执行步进控制
  doSweep(leftServo, leftCurrentPositionDegrees, leftTargetPositionDegrees, leftLastSweepCommand, g_stepDelayMs);
  doSweep(rightServo, rightCurrentPositionDegrees, rightTargetPositionDegrees, rightLastSweepCommand, g_stepDelayMs);

#endif
}

