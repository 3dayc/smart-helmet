#ifndef SMARTHEAD_MRT_CODINGSHIELD_H
#define SMARTHEAD_MRT_CODINGSHIELD_H

#include <SoftwareSerial.h> 
#include <MsTimer2.h> 

#ifdef NEOPIXEL
  #include <Adafruit_NeoPixel.h>
  #define LED_PIN     2
  #define LED_COUNT   2  
  #define LED_EYE     0
  #define LED_LIPS   1
  #define LED_ALL     2
  
  uint8_t brightness = 250;  
  int8_t  directionVal = -10;  
  
  Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
#endif
#ifdef VL53LOX
  #include "Adafruit_VL53L0X.h"
  Adafruit_VL53L0X lox = Adafruit_VL53L0X(); 
#endif  
#ifdef DFIMP3  
  #include <DFPlayer_Mini_Mp3.h>
#endif  

// 쉴드 보드의 핀레이아웃
#define TOUCH_SEN         A0
#define MIC_SEN           A3
#define CDS_SEN           A6 
#define BAT_SEN           A7

#define BOT_TX            5
#define MP3_TX            7
#define MP3_BUSY          8   

/*
// 확장포트 현재 미사용
#define LEFT_LEG_D        3
#define LEFT_LEG_A        A1
#define RIGHT_LEG_D       4
#define RIGHT_LEG_A       A2
#define MIC_SEN           A3
*/

// 듀얼 소프트웨어 시리얼을 사용합니다. 
SoftwareSerial botSerial(0, BOT_TX); // RX, TX
SoftwareSerial mp3Serial(0, MP3_TX); // RX, TX

// 시간
uint32_t  refTm=0,pTm=0,lastTm=0;
uint16_t  cdsVal, distVal,batVal,touchVal,mp3Val,micVal;
uint16_t  mSec100,oneSec;
uint16_t  tictokCount = 0;   

void readSensor()
{
  cdsVal = analogRead(CDS_SEN);
  batVal = analogRead(BAT_SEN);
  touchVal=analogRead(TOUCH_SEN) ? 1:0;  
  micVal  =analogRead(MIC_SEN);
  distVal =50; // closer
#ifdef VL53LOX  
  lox.rangingTest(&measure, false);     
  if (measure.RangeStatus != 4) distVal = measure.RangeMilliMeter / 10; //for cm    
#endif
#ifdef DEBUG_SEN
  Serial.print("cdsVal = ");Serial.print(cdsVal);  
  //Serial.print("\t batVal = ");Serial.print(batVal / 4.7);  
  Serial.print("\t touchVal = ");Serial.print(touchVal);  
  Serial.print("\t micVal = ");Serial.print(micVal);  
  Serial.print("\t distVal = ");Serial.println(distVal);    
#endif
}
void actionRobot(unsigned char exeIndex, bool flag)
{
  unsigned char exeCmd[15] = {0xff, 0xff, 0x4c, 0x53, 0x00, 
                            0x00, 0x00, 0x00, 0x30, 0x0c, 0x03,
                            0x01, 0x00, 100, 0x54};            
  exeCmd[11] = exeIndex;
  exeCmd[14] = 0x00;    // checksum
    
  for (int i=6;i<14;i++)
    exeCmd[14]+=exeCmd[i];    
  
  botSerial.write(exeCmd,15);
  delay(100);
}
void actionDelay(unsigned char d)
{
  switch(d){
    
    case 1:
    case 2:  case 3:  case 4 : case 5:
    case 6:  case 7:  case 8 : case 9:
    case 10: case 11: case 12: case 13 :
    case 22 : case 23 : case 28 : case 29 :
    case 30 : case 39: case 40 : case 51:
    case 55: case 57 : case 59: case 75:
    case 76 : case 77 : case 78 : case 79 :
    case 80: case 81: case 82 : case 83 :
    case 86: case 88 : case 102 : case 104 :
        
    delay(1000);
    break;

    case 24: case 25 : case 26 : case 27: 
    case 31 :case 32: case 33: case 34 :
    case 35: case 36 : case 41: case 42:
    case 43 : case 44 : case 45 : case 46:
    case 47 : case 52: case 56 : case 58 :
    case 60 : case 61 : case 62 : case 63:
    case 64: case 65 : case 66: case 67 :
    case 68 : case 69: case 85:case 87:
    case 89 : case 90 : case 91: case 92 :
    case 93: case 98: case 99 : case 100:
    case 101: case 103: case 105 : case 106 :
    case 107: case 108: case 109: case 110:
    case 111: case 112:
    
    delay(2000);
    break ;

    case 48 :case 49: case 50 :
    delay(2500); 
    break;

    case 70 : case 71: case 72 :
    delay(3000);
    break;

    case 240 : delay(5000); break;
   
    case 17: case 18 : delay(15000);
    break;

    case 16: case 19: case 84: case 94:
    case 115: case 116: case 241 :
    delay(7000);
    break;

    default :break; 
   }  
}

void actionRobot(unsigned char exeIndex)
{
unsigned char exeCmd[15] = {0xff, 0xff, 0x4c, 0x53, 0x00, 
                              0x00, 0x00, 0x00, 0x30, 0x0c, 0x03, // 0x0c=���� ���� ��� 0x03=�Ķ��Ÿ�� 
                              0x01, 0x00, 100, 0x54};             // Index 01 ��ɾ� ����
/*                             |      |    |    `---Check sum
                               |      |    `--------�ӵ�      �Ķ�1
                               |      `-------------�����ð�  �Ķ�2
                               `--------------------Index    �Ķ�3
*/  
// 14, 15, 20, 21, 37, 38, 53, 54, 73, 74
// 95, 96, 97,113, 114 ,117, 118
if(exeIndex==14 || exeIndex==15 || exeIndex==20  || exeIndex==21 || exeIndex== 37
    ||exeIndex== 38 || exeIndex== 53 || exeIndex== 54 || exeIndex== 73 || exeIndex== 74
    || exeIndex== 95 || exeIndex== 96 || exeIndex== 97 || exeIndex== 113 || exeIndex== 114
    || exeIndex== 117 || exeIndex== 118) return ;

  exeCmd[11] = exeIndex;
  exeCmd[14] = 0x00;    // checksum
    
  for (int i=6;i<14;i++)
    exeCmd[14]+=exeCmd[i];    
  
  botSerial.write(exeCmd,15);
  delay(100); 

  actionDelay(exeIndex);
}

uint8_t waitMp3(uint16_t nSec)
{
#ifdef DFIMP3  
  pTm = refTm;    
  delay(200);
  while( (refTm - pTm < nSec) )
  { 
    delay(200);    
    mp3Val = digitalRead(MP3_BUSY);
    if (mp3Val == 1) return 0;    
  }  
#endif  
  return 1;

}

uint8_t playMp3(uint16_t TrackNo,uint16_t delaySec)
{
#ifdef DFIMP3    
  mp3_stop();  
  delay(1);
  
  delay(100);
  delay(1);
  mp3_play(TrackNo);
  
  if (!waitMp3(delaySec)) 
  {  
    mp3_stop();  
    delay(1);
    actionRobot(0);  
    delay(2000);    
    return 0;
  }
  
  mp3_stop();
  delay(1);  
#endif  
  return 1;
}

void setColorLED(uint8_t what, uint8_t rr, uint8_t gg, uint8_t bb)
{
  if (what == 2 )
  {
    strip.setPixelColor(0, strip.Color( rr,gg,bb ));
    strip.setPixelColor(1, strip.Color( rr,gg,bb ));
  }
  else
  {
    strip.setPixelColor(what, strip.Color( rr,gg,bb ));
  } 
}
void dimmingLed(uint16_t delayMs )
{
#ifdef NEOPIXEL          
  if (directionVal < 0 && brightness <= 10){      
    directionVal = 10;    
  }
  else if (directionVal > 0 && brightness >= 250){
    directionVal = -10; 
  }  
  // apply dimming    
  brightness += directionVal;  
  strip.setBrightness(brightness);
  strip.show(); 
  delay(delayMs);
#endif  
}
void toggleLed(uint8_t what , uint16_t delayMs, uint8_t times)
{
#ifdef NEOPIXEL      
  for (int i=0;i<times;i++)
  {
    strip.setBrightness(5);    
    strip.show(); 
    delay(delayMs);      
    strip.setBrightness(255); 
    strip.show(); 
    delay(delayMs);   
  }  
#endif  
}
void timerHandler() 
{
  tictokCount++;               
  if(tictokCount > 9){    
    tictokCount = 0;     
    refTm++;
  }
 
  TCNT2 = 130;           //Reset Timer to 130 out of 255
  TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
}
void initHardware()
{
  botSerial.begin(115200); // for Robot  
#ifdef DFIMP3      
  mp3Serial.begin(9600);   // MP3      
  mp3_set_serial (mp3Serial);
  delay(1);  
  mp3_set_volume (26);  
  delay(1);
#endif
#ifdef NEOPIXEL      
  strip.begin();  
  strip.show();
  strip.setBrightness(250);  
  strip.setPixelColor(LED_EYE, strip.Color(255,0,0));
  strip.setPixelColor(LED_LIPS, strip.Color(0,255,0));
  strip.show();
  delay(200);
  strip.setPixelColor(LED_EYE, strip.Color(0,255,0));
  strip.setPixelColor(LED_LIPS, strip.Color(255,0,0));
  strip.show();
  delay(200);
  strip.setPixelColor(LED_EYE, strip.Color(255,0,0));
  strip.setPixelColor(LED_LIPS, strip.Color(0,255,0));
  strip.show();
  delay(200);
#endif  
#ifdef VL53LOX  
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));    
  }  
#endif  
  Serial.println("8");   
  pinMode(TOUCH_SEN ,INPUT);
  pinMode(CDS_SEN   ,INPUT);
  pinMode(BAT_SEN   ,INPUT);
  pinMode(MP3_BUSY  ,INPUT); 
  pinMode(13        ,OUTPUT);
  
  
  Serial.println("=============================================");
  Serial.println("== Program Started by Smart helmet         ==");
  Serial.println("==                    myrobottime.co.kr    ==");  
  Serial.println("==                    2020  1 31           ==");  
  Serial.println("=============================================");
   
  actionRobot(0);
  delay(1000);
  MsTimer2::set(100, timerHandler); // 100ms period
  MsTimer2::start();   
  refTm = 0;  
}
#endif // SMARTHEAD_MRT_CODINGSHIELD_H
