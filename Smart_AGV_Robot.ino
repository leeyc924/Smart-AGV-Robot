  #include <PRIZM.h>
  #include <Wire.h>
  #include "Adafruit_TCS34725.h"

  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
  #define reset digitalRead(2) // 감지 :0, 미감지 =1(pull up 필수)
  #define line analogRead(A1)  // 가운데 라인감지센서
  #define line_left analogRead(A2) //왼쪽 라인감지센서
  #define line_right analogRead(A3) //오른쪽 라인감지센서

  int art_color = 0; //컬러 색
  int lineposition = 1; //현재라인
  int Bottom_object[4] = {2,2,2,2}; //오브젝트 수량
  int Top_object[4] = {0,0,0,0}; // 운반오브젝트 수량
  int avg=(960+32)/2;
      
  PRIZM prizm;
  EXPANSION ex;  
  
void wheel(int x, int y, int z);
void linefind(int x,int y,int z) // 라인찾는 함수
{
  while(1)
  {
  if(line>150)
  {wheel(0,0,0);delay(10);break;}
  else
  {wheel(x,y,z);delay(1);}
  }
}

void movelinefind(int x)  //왼쪽으로 :1  오른쪽으로 :2
{
  if(x==1)
  {
    wheel(-60,0,0);
    delay(1570);
    linefind(-30,-5,0);
  }
  else if(x==2)
  {
    wheel(60,0,0);
    delay(1570);
    linefind(30,-5,0);
  }
}

void linetracer()  //라인트레이싱
{
  int sp=0;
   while(1)  
  {    
    if((line_left>avg)&&(line_right>avg))     
    {
      wheel(0,0,0);delay(10);break;
    }
    else if((line_left>avg)&&(line_right<avg))
    {
      wheel(0,15,-8);delay(1);
    }
    else if((line_right>avg)&&(line_left<avg))
    {
      wheel(0,15,8);delay(1);
    }
    else
    {
    sp=(line-avg)/10;
    wheel(sp/10,35,sp/4);
    delay(1);  
    }
  }
}

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

void wheel1(int a1,int a,int b1,int b,int c1,int c,int d1,int d)  //옴니휠 타겟이동
{
  ex.resetEncoders(1);
  ex.resetEncoders(2);
  ex.setMotorTargets(1,a1,a,b1,b);
  ex.setMotorTargets(2,c1,c,d1,d);
}

void RGB_match(void)     //컬러센서 감지
{
    uint16_t clear, red, green, blue;
    int i =0;
    for(i=0;i<2;i++)
    {
    tcs.getRawData(&red, &green, &blue, &clear);
    if(red > green & red < blue ) // 빨강색 = 1
    {art_color =1;}
    else if(blue > red & blue < green ) // 초록색 = 2
    {art_color =2;}
    else if(green > red & green < blue) // 퍼렁색 =3
    {art_color =3;}
    else
    {art_color = 1;}
    }
}

void object_carry(void)  //오브젝트 들고 이동
{
  while(1)
  {
    int i;
      if((lineposition ==1)&&(art_color ==1))   //1번라인에서 빨강
      {
         linetracer();
         if(Top_object[1] ==0)  //빨강 처음 내려놀때
          { 
            Top_object[1] =1;
            liftmove(3,1);
            objectDown(2);
            lineposition =1;
          }
         else                  //빨강 두번째 내려놀때
          {
            Top_object[1] =2;
            liftmove(4,1);
            objectDown(2);
            lineposition =1;
          }
      break;
      }
     else if((lineposition ==2)&&(art_color ==2))//2번라인에서 초록
          {    
          linetracer();
          if(Top_object[2] ==0) //초록 처음 내려놀때
            {
              Top_object[2] =1;
              liftmove(3,1);
              objectDown(2);
              lineposition =2;
            }
          else                 //초록 두번째 내려놀때
            {
              Top_object[2] =2;
              liftmove(4,1);
              objectDown(2);
              lineposition = 2;
            }
      break;
      }
    else if((lineposition ==3)&&(art_color ==3))  //3번라인에서 파랑
      {
          linetracer();
          if(Top_object[3] ==0) //파랑 처음 내려놀때ff
          {
            Top_object[3] =1;
            liftmove(3,1);
            objectDown(2);
            lineposition =3;
          }
          else                //파랑 두번째 내려놀때
          {
            Top_object[3] =2;
            liftmove(4,1);
            objectDown(2);
            lineposition = 3;
          }   
      break;
      }
    else   // 라인과 색상이 미일치 할때
    {
        if(lineposition ==1) //1번 라인미일치
        {
        lineposition =2;
        movelinefind(1);      
        } 
    else if(lineposition ==2)//2번 라인 미일치 좌우비교 맞는쪽이동
    {
      if(art_color ==1) // 1번라인으로 이동
      {
        lineposition =1;
        movelinefind(2);
      }
      else
      {
        lineposition =3;//3번라인으로 이동
        movelinefind(1);
      }
    }
    else //3번라인 미일치
    {
      lineposition =2;
      movelinefind(2);
    }
   }
    
  }
}

void objectDown(int a)   //오브젝트 내려놓고 뒤돌기
{
  wheel(0,-40,0);
  delay(1300);
  if(a==2)
  {
    while(1)
    {
      if(line_right>avg)
      {
      wheel(0,0,0);
      delay(300);
      break;
      }
      wheel(0,0,25);
      delay(1);
    }
    linefind(25,0,0);
  }
  else if(a==1)
  {
    while(1)
    {
      if(line_left>avg)
      {
      wheel(0,0,0);
      delay(300);
      break;
      }
      wheel(0,0,-25);
      delay(1);
      }
    linefind(0,0,-25);
  }
}

void Carry_after(void)  //오브젝트에 따른 경우의수
{
  int i;
  if(lineposition==1 && Bottom_object[1]==2)
  {liftmove(2,1);lineposition=1;}
  else if(lineposition==1 && Bottom_object[1]==1)
  {liftmove(1,1);lineposition=1;}
  else if(lineposition==1 && Bottom_object[1]==0)
  {
    if(Bottom_object[2]==2)
    {movelinefind(2); liftmove(2,1);lineposition=2;}
    else if(Bottom_object[2]==1)
    {movelinefind(2); liftmove(1,1);lineposition=2;}
    else if(Bottom_object[2]==0 && Bottom_object[3]==2)
    {
      for(i=0;i<2;i++)
      {movelinefind(2);} 
      liftmove(2,1);
      lineposition=3;
    }
    else if(Bottom_object[2]==0 && Bottom_object[3]==1)
    {
      for(i=0;i<2;i++)
      {movelinefind(2);} 
      liftmove(1,1);
      lineposition=3;
    }
    else if(Bottom_object[2]==0 && Bottom_object[3]==0)
    {Yellow_object();lineposition=1;}
  }
  else if(lineposition==2 &&  Bottom_object[2]==2)
  {liftmove(2,1);lineposition=2;}
  else if(lineposition==2 && Bottom_object[2]==1)
  {liftmove(1,1);lineposition=2;}
  else if(lineposition==2 && Bottom_object[2]==0)
  {  
    if(Bottom_object[1]==1)
    {movelinefind(1); liftmove(1,1);lineposition=1;}
    else if(Bottom_object[1]==0 && Bottom_object[3]==2)
    {movelinefind(2); liftmove(2,1);lineposition=3;}
    else if(Bottom_object[1]==0 && Bottom_object[3]==1)
    {movelinefind (2);liftmove(1,1);lineposition=3;}
    else if(Bottom_object[1]==0 && Bottom_object[3]==0)
    {Yellow_object();lineposition=2;}
  }
  else if(lineposition==3 && Bottom_object[3]==2)
  {liftmove(2,1);lineposition=3;}
  else if(lineposition==3 && Bottom_object[3]==1)
  {liftmove(1,1);lineposition=3;}
  else if(lineposition==3 && Bottom_object[3]==0)
  {
    if(Bottom_object[1]==1)
    {   
      for(i=0;i<2;i++)
      {movelinefind(1);}
      liftmove(1,1);
      lineposition=1;
    }
    else if(Bottom_object[1]==0 && Bottom_object[2]==2)
    {movelinefind(1);liftmove(2,1);lineposition=2;}
    else if(Bottom_object[1]==0 && Bottom_object[2]==1)
    {movelinefind(1);liftmove(1,1);lineposition=2;}
    else if(Bottom_object[1]==0 && Bottom_object[2]==0)
    {Yellow_object();lineposition=3;}
  }
  Bottom();
}

void Bottom(void)  //오브젝트 수량 파악
{
  if(lineposition==1&&Bottom_object[1]==2)
  {Bottom_object[1]=1;}
  else if(lineposition==1&&Bottom_object[1]==1)
  {Bottom_object[1]=0;}
  else if(lineposition==1&&Bottom_object[1]==0)
  {Bottom_object[1]=0;}
  else if(lineposition==2&&Bottom_object[2]==2)   
  {Bottom_object[2]=1;}
  else if(lineposition==2&&Bottom_object[2]==1)
  {Bottom_object[2]=0;}
  else if(lineposition==2&&Bottom_object[2]==0)
  {Bottom_object[2]=0;}
  else if(lineposition==3&&Bottom_object[3]==2)
  {Bottom_object[3]=1;}
  else if(lineposition==3&&Bottom_object[3]==1)
  {Bottom_object[3]=0;}
  else if(lineposition==3&&Bottom_object[3]==0)
  {Bottom_object[3]=0;}
}

void Yellow_object(void)  //노란색 오브젝트 옮기기
{  
  liftmove(3,1);
  int i;
  if(lineposition==1)
  { 
    for(i=0;i<2;i++)
    {
      movelinefind(2);
    }  
    linetracer();
  }
  else if(lineposition==2)
  {
    movelinefind(2);
    linetracer();
  }
  else if(lineposition==3)
  {linetracer();}
  wheel(60,0,0);
  delay(1300);
  linefind(0,0,30); 
  linetracer();  
  liftmove(5,1);
  wheel(0,-40,0);
  delay(1500);
  wheel(60,0,0); // moveline
  delay(1600);
  linefind(30,-5,0);;
  linetracer();
  liftmove(4,1);
  wheel(0,-40,0);
  delay(1300);
  linefind(20,-10,0);
  liftmove(3,1);
  int sp=0;
   while(1)  
  {    
    if((line_left>avg)&&(line_right>avg))     
    {
      wheel(0,0,0);delay(10);break;
    }
    else if((line_left>avg)&&(line_right<avg))
    {
      wheel(0,15,-8);delay(1);
    }
    else if((line_right>avg)&&(line_left<avg))
    {
      wheel(0,15,8);delay(1);
    }
    else
    {
    sp=(line-avg)/10;
    wheel(-sp/10,35,-sp/4);
    delay(1);  
    }
  }
  liftmove(6,1);
  wheel(0,-40,0);
  delay(1300);
  while(1)
  {
    if(line_left>avg)
    {
      wheel(0,0,0);
      delay(300);
      break;
    }
     wheel(0,0,-25);
     delay(1);
  }
  linefind(-25,0,0);
  linetracer();
  wheel(-30,0,0);
  delay(500);
  wheel1(320,7300,320,7300,320,-7300,320,-7300);
  delay(7500);
  while(1)
  {
    if(line_left>avg||line_right>avg)
    {
      wheel(0,0,0);delay(10);break;
    }
    else
    {
     wheel(0,50,0);
     delay(1);
    }
  }    
  /*while(1)
  {
    if(line_right>150||line_left>150)
    {wheel(0,0,0);delay(10);break;}
    else
    {wheel(0,30,0);delay(10);}
  }
  wheel(0,30,0);
  delay(700);
  wheel(0,0,20);
  delay(500);
  linefind(0,0,20);
  linetracer();
  wheel1(250,-2000,250,2000,250,2000,250,-2000);
  delay(1990);
  while(1)
  {
    if(line_left>150)
    {
      wheel(0,0,0);delay(10);break;
    }
    else
    {
     wheel(-30,-30,0);
     delay(10);
    }
  }
  for(i=0;i>-40;i--)
  {
    wheel(0,0,i);
    delay(10);
  }
  for(i=-35;i<-5;i++)
  {
    wheel(0,0,i);
    delay(10);
  }*/
  liftmove(1,1);
}   

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
      prizm.setMotorTarget(1,720,-980);
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<=-960) && (prizm.readEncoderCount(1)>= -1000))
        {break;}
        else {delay(1);}
      }
      break;
      
    case 3:  //위에1층
      prizm.setMotorTarget(1,720,-4700);
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<= -4680) && (prizm.readEncoderCount(1)>= -4720))
        {break;}
        else {delay(1);}
      } 
      break;
      
    case 4:  //위에2층
      prizm.setMotorTarget(1,720,-5720); 
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<= -5700) && (prizm.readEncoderCount(1)>= -5740))
        {break;}
        else {delay(1);}
      }
      break;
      
    case 5:  //최대높이
       prizm.setMotorTarget(1,720,-6900); 
      while(cnt == 1)
      {
        if((prizm.readEncoderCount(1)<= -6880) && (prizm.readEncoderCount(1)>= -6920))
        {break;}
        else {delay(1);}
      }
      break;    
    
      case 6:  //노란색살짝들기
      prizm.setMotorTarget(1,720,-5350); 
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
  Wire.begin();
}

void loop() 
{
  liftmove(1,1);
  wheel1(0,0,400,-4200,0,0,400,4200);
  delay(3500);
  movelinefind(2);
  int i;
  for(i=0;i<2;i++)
  {
    Carry_after();
    linetracer();
    liftmove(5,1);
    RGB_match();
    objectDown(1);
    object_carry();
  }
  Yellow_object();
  prizm.PrizmEnd();
}
