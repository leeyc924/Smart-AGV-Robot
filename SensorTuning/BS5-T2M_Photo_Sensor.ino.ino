#include <PRIZM.h>
#define reset digitalRead(2) // 감지 :0, 미감지 =1(pull up 필수)
PRIZM prizm;

void liftreset(void)  //리프트 초기 위치로 이동 후 Encoder 리셋
{   
    while(1)
    {
        if(reset==0)
        {
            prizm.setMotorPower(1,0);
            prizm.resetEncoder(1);
            delay(100);
            break;
        }
        else 
        {
            prizm.setMotorPower(1,50);
            delay(1); 
        }  
    }
}

void liftmove(int dot, int cnt) // dot == 이동위치 , cnt는 제자리 완료시간 확인 기능 (제자리 이동은 1, 무빙중 이동은 0)
{             
   switch(dot)
   {
    case 1://초기화 1층
      liftreset();
      break;

    case 2:  // 2층
      prizm.setMotorTarget(1,500,-980);
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<=-960) && (prizm.readEncoderCount(1)>= -1000))
        {break;}
        else {delay(1);}
      }
      break;
      
    case 3:  //위에1층
      prizm.setMotorTarget(1,500,-4600);
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<= -4580) && (prizm.readEncoderCount(1)>= -4620))
        {break;}
        else {delay(1);}
      } 
      break;
      
    case 4:  //위에2층
      prizm.setMotorTarget(1,500,-5700); 
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<= -5680) && (prizm.readEncoderCount(1)>= -5720))
        {break;}
        else {delay(1);}
      }
      break;
      
    case 5:  //최대높이
       prizm.setMotorTarget(1,500,-6900); 
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<= -6880) && (prizm.readEncoderCount(1)>= -6920))
        {break;}
        else {delay(1);}
      }
      break;    
    
      case 6:  //노란색살짝들기
      prizm.setMotorTarget(1,500,-5350); 
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<= -5330) && (prizm.readEncoderCount(1)>= -5370))
        {break;}
        else {delay(1);}
      }
      break;
   }
}

void setup() 
{    
  prizm.PrizmBegin();
  Serial.begin(9600);
  prizm.resetEncoder(1);
  pinMode(2, INPUT_PULLUP);      
}

void loop() 
{
  liftmove(1,1);
  liftmove(3,1);
  liftmove(5,1);
  liftmove(2,1);
  liftmove(4,1);
  liftmove(6,1);
  liftmove(2,1);
  liftmove(1,1);
  prizm.PrizmEnd();
}
