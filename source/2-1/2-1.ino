// 사용할 센서와 기능을 정의 하세요 
#define NEOPIXEL    // rgbw LED 입과 눈
//#define VL53LOX   // 거리센서
//#define DFIMP3    // MP3플레이어 
//#define DEBUG_SEN // 센서의 측정값을 보여줍니다.

#include "smartHelmet.h" // 스마트핼맷의 공통함수를 사용합니다.
int dayTime = 400; // 낮과 밤을 구분하기위한 임의의 숫자 0~1023까지 밝을수록 높음
void setup() {  
  Serial.begin (115200); // for debugging
  initHardware();        // 이것은 반드시 필요합니다.   
}

void loop() {   
  
  readSensor();  // 스마트헬멧의 센서를 모두 읽어 들입니다.
  if (cdsVal < dayTime)
  {    
    Serial.println("Night Time");    
    setColorLED(LED_EYE, 255,  255,255); // rgbr 가장 밝게 흰색
    setColorLED(LED_LIPS,  0,  255,  0); // rgb    
    toggleLed  (LED_EYE, 200,  3);       // 깜빡임
    delay(500);
  }
  else
  {
    Serial.println("Day Time");    
    setColorLED(LED_EYE ,255,  0,  0); // rgb
    setColorLED(LED_LIPS,  0,255,  0); // rgb
    dimmingLed (100); // 100ms간격으로 점차 어두워졌다가 밝아지는 서비스 함수 
  }    
}
