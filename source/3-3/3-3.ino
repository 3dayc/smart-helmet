// 사용할 센서와 기능을 정의 하세요 
//#define NEOPIXEL    // rgbw LED 입과 눈
//#define VL53LOX   // 거리센서
#define DFIMP3    // MP3플레이어 
//#define DEBUG_SEN // 센서의 측정값을 보여줍니다.

#include "smartHelmet.h" // 스마트핼맷의 공통함수를 사용합니다.
int tooLoud = 400; // 0~1023까지 소리가 클수록 높음

void setup() {  
  Serial.begin (115200); // for debugging
  initHardware();        // 이것은 반드시 필요합니다.     
  actionRobot(1);        // 로봇의 기본 모드 준비 자세  
}

void loop() {   
  readSensor();  // 스마트헬멧의 센서를 모두 읽어 들입니다.
  if (micVal > tooLoud){
    Serial.println("음원에 맞춰 춤을 춥니다.");
    actionRobot(4,LOW);//2016K1 모션을 다운로드 되어 있고 4번 댄스를 합니다.
    playMp3(1604,500);      // microSDCard에 저장된 파일명 4232.mp3를 500초이내에 플레이 함
    actionRobot(0); //음악이 끝났음. 정지
    actionRobot(3); //하이 (손 흔들어 인사)
    actionRobot(11);//차렷
  }
}
