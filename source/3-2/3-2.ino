// 사용할 센서와 기능을 정의 하세요 
//#define NEOPIXEL    // rgbw LED 입과 눈
#define VL53LOX   // 거리센서
//#define DFIMP3    // MP3플레이어 
//#define DEBUG_SEN // 센서의 측정값을 보여줍니다.

#include "smartHelmet.h" // 스마트핼맷의 공통함수를 사용합니다.

int distNear = 40; // 40cm보다 가까우면 
int distFar  = 60; // 60cm보다 멀면

void setup() {  
  Serial.begin (115200); // for debugging
  initHardware();        // 이것은 반드시 필요합니다.     
}

void loop() {   
  readSensor();  // 스마트헬멧의 센서를 모두 읽어 들입니다.
  if (distVal < distNear)  {    
    Serial.println("너무 가깝습니다. 뒤로 갑니다.");
    actionRobot(9); // 후진 준비
    actionRobot(11);// 후진
    actionRobot(10);// 후진 종료
  }  else if (distVal > distFar)  {
    Serial.println("로봇이 다가갑니다.");
    actionRobot(2); // 전진 준비
    actionRobot(3); // 전진
    actionRobot(4); // 전진 종료
  }  else  {
    actionRobot(1); // 차렷
  }
}
