
#ifdef ARDUINO_MY_COUNTER
  #define LEFT_A  20      // 2
  #define LEFT_B  21      // 3
  #define RIGHT_A  18     // 5
  #define RIGHT_B  19     // 4

  void initEncoder();
  void leftEncoderEventA();   //中断函数
  void leftEncoderEventB();
  void rightEncoderEventA();
  void rightEncoderEventB();
#endif
   
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();

