
#ifdef USE_BASE

#ifdef TB6612_MOTOR_DRIVER

  void initMotorController() {

    digitalWrite(STBY, HIGH);
    pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
    pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
    pinMode(LEFT_MOTOR_PWM, OUTPUT);
    pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
    pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
    pinMode(RIGHT_MOTOR_PWM, OUTPUT);
    
  }
  
  void setMotorSpeed(int i, int spd) {
    unsigned char reverse = 0;
    if (spd < 0) {
      spd = -spd;
      reverse = 1;
    }
    if (spd > 255)
      spd = 255;
    
    if (i == LEFT) { 
      if (reverse == 0) {
        digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
        analogWrite(LEFT_MOTOR_PWM, spd);
      }
      else if (reverse == 1) {
        digitalWrite(LEFT_MOTOR_FORWARD, LOW);
        digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
        analogWrite(LEFT_MOTOR_PWM, spd);
      }
    }
    else {
      if (reverse == 0) {
        digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
        analogWrite(RIGHT_MOTOR_PWM, spd);
      }
      else if (reverse == 1) {
        digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
        digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
        analogWrite(RIGHT_MOTOR_PWM, spd);
      }
    }
  }
  
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
//-------------------------------------------------------------------------------------------------------------------------------------
#else
  #error A motor driver must be selected!
#endif

#endif
