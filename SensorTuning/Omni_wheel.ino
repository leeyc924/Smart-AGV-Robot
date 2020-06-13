 
  #include <PRIZM.h>
  
  PRIZM Robowell;
  EXPANSION ex;
  
  void wheel(int x, int y, int z) // Omni wheel 이동공식
  {
    int A = 0, B = 0, C = 0, D = 0;
  
    A = (x * 0.5) + (y * 0.5) + (z * 0.841471);
    B = (x * 0.5 * -1) + (y * 0.5) + (z * 0.841471);
    C = (x * 0.5 * -1) + (y * 0.5 * -1) + (z * 0.841471);
    D = (x * 0.5) + (y * 0.5 * -1) + (z * 0.841471);
 
    ex.setMotorPowers(1, A, B);
    ex.setMotorPowers(2, C, D);
  }
  
  //////////////////////////////////////////////////////////////////////// main
  
  void setup()
  {
    Robowell.PrizmBegin();
    Serial.begin(9600);
    Robowell.resetEncoder(1);
  }
  
  void loop()
  {
   wheel(0,60,0);
   delay(3000);
   wheel(0,-60,0);
   delay(3000);
   Robowell.PrizmEnd(); 
  }
  
