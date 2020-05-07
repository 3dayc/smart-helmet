// 사용할 센서와 기능을 정의 하세요 
#define NEOPIXEL    // rgbw LED 입과 눈
//#define VL53LOX   // 거리센서
#define DFIMP3    // MP3플레이어 
#define DEBUG_SEN // 센서의 측정값을 보여줍니다.

#include "smartHelmet.h" // 스마트핼맷의 공통함수를 사용합니다.
int touchedSen = 1;      // 0 혹은 1
void setup() {  
  Serial.begin (115200); // for debugging
  initHardware();        // 이것은 반드시 필요합니다.     
}

void loop() {   
  readSensor();  // 스마트헬멧의 센서를 모두 읽어 들입니다.
  if (touchVal == touchedSen)
  {    
    Serial.println("머리를 쓰다듬었습니다");
    playMp3(4232,20); // microSDCard에 저장된 파일명 4232.mp3를 20초이내에 플레이 함
  }
}
