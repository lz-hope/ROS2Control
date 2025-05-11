
#ifdef TB6612_MOTOR_DRIVER
  #define LEFT_MOTOR_BACKWARD  5    //AIN1
  #define LEFT_MOTOR_FORWARD   4    //AIN2
  #define LEFT_MOTOR_PWM 9          //PWMA
  
  #define RIGHT_MOTOR_BACKWARD 6    //BIN1
  #define RIGHT_MOTOR_FORWARD  7    //BIN2
  #define RIGHT_MOTOR_PWM 8         //PWMB
  
  #define STBY 12                   //STBY
#endif

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
