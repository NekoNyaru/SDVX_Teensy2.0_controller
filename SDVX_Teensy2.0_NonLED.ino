/*
CC BY-NC-SA 2.0 KR에 따라 이용할 수 있습니다
*/

#include <Adafruit_NeoPixel.h>
#include <Encoder.h>
#include <math.h>

//▼ 부품
  #define ro 400          // 로터리인코더 해상도
  #define stripLED0 13    // 서클 네오픽셀 LED
//▲

//▼ 핀 선언
  //노브핀 선언
  #define nov1 7
  #define nov2 8
  #define nov3 5
  #define nov4 6          // 얘네들은 왠만하면 고정
  //네오픽셀핀선언
  #define neo1 9
  #define neo2 10
  //볼륨노브 선언
  #define Vol1 23
  #define Vol2 22
  #define FVRIT 11

  #define nov_button_sensitivity 100 //노브버튼모드 정지 민감도

  // 버튼핀, LED핀 선언
  int bu[] = {0, 3, 18, 24, 16, 12, 20};            // 왼쪽부터 0번(start) ~ 6번 (FX_R) 
  int le[] = {1, 19, 2, 4, 17, 13, 21};             // 스위치 안에 들어가는 LED 핀

  //개수 선언
  const int num_pins = sizeof(bu) / sizeof(bu[0]);  // 핀 개수 추출 (7)
//▲ 

//▼ 기본 세팅 값
  #define novdeadzone 0.3                           // 노브 데드존
  #define ledlight 1                                // 노브 회진시 점등되는 LED수량
  #define novledofftime 1000                        // 노브 LED 지속시간 (ms)
  #define sleepmode 1000                            // 슬립모드 LED 작동시간

//▲

// 버튼 상태 bool값 지정
  boolean b_stat[] = {false, false, false, false, false, false, false};

//▼ 노브 관련 설정값
  //엔코더 선언
  Encoder novl(nov1, nov2);
  Encoder novr(nov3, nov4);
  Encoder volnov(Vol1, Vol2);

  //노브 속도
  double novspd_l = 0.0;
  double novspd_r = 0.0;

  //노브 위치 360도 값
  double novro_l = 0.0;
  double novro_r = 0.0;
//▲ 

//▼ LED 관련 구문
  //켜져야 하는 노브 led 주소
  short ledon1;
  short ledon2;
  
  //노브led 소등 딜레이
  int novllastmovetime = 0;
  int novrlastmovetime = 0;

  //네오픽셀 선언
  Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(stripLED0, neo1, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(stripLED0, neo2, NEO_GRB + NEO_KHZ800);
//▲

//러닝타임
int runningtime = 0;

//볼륨, 즐겨찾기관련
  boolean favrit = false;
  boolean voldown = false;
  int voL = 0;

void setup() {
  // put your setup code here, to run once:
  //버튼 input 선언
  for (int i = 0; i < 7; i++) {
    pinMode(bu[i], INPUT_PULLUP);
  }

  //LED OUTPUT 선언
  for (int i = 0; i < 7; i++) {
    pinMode(le[i], OUTPUT);
  }

  //네오픽셀 초기화
  strip1.begin();
  strip2.begin();
  
  //조이스틱 초기화
  Joystick.useManualSend(true);

  Serial.begin(9600);

  //볼륨엔코더 클릭 선언
  pinMode(FVRIT, INPUT_PULLUP);

}

void loop() {
  
  runningtime = millis();

    //버튼값 b_stat에 넣기
  for (int ch = 0; ch < num_pins; ch++) {
    if (digitalRead(bu[ch]) == HIGH) {
      b_stat[ch] = false;
    } else {
      b_stat[ch] = true;
    }
  }

        if (b_stat[0] == true) { // START
      Joystick.button(1, 1);
      digitalWrite(le[0], HIGH);
    } else {
      Joystick.button(1, 0);
      digitalWrite(le[0], LOW);
    }

    for (int ch = 1; ch <= 4; ch++) { // BT 1 ~ 4
      if (b_stat[ch] == true) {
        Joystick.button(ch + 1, 1);
        digitalWrite(le[ch], HIGH);
      } else {
        Joystick.button(ch + 1, 0);
        digitalWrite(le[ch], LOW);
      }
    }
    
    for (int ch = 5; ch < 7; ch++) { //FX-L , R
      if (b_stat[ch] == true) {
        Joystick.button(ch + 1, 1);
        digitalWrite(le[ch], HIGH);
      } else {
        Joystick.button(ch + 1, 0);
        digitalWrite(le[ch], LOW);
      }
    }
  
  //-------------------------------------------------------------------------------// 노브 사전 세팅
    //현 노브 값으로 노브 속도 측정
    novspd_l = novl.read();
    novl.write(0);
    novspd_r = novr.read();
    novr.write(0);
    voL = volnov.read();
    volnov.write(0);

    //노브속도를 360도 값으로 변환
    novspd_l = novspd_l / ro / 4 * 360;
    novspd_r = novspd_r / ro / 4 * 360;

    //데드존
    if(novspd_l > 0){
      if(novspd_l <= novdeadzone){
        novspd_l = 0;
      }
    }
    else if (novspd_l < 0) {
      if(novspd_l >= novdeadzone * -1){
        novspd_l = 0;
      }
    }
    if(novspd_r > 0){
      if(novspd_r <= novdeadzone){
        novspd_r = 0;
      }
    }
    else if (novspd_r < 0) {
      if(novspd_r >= novdeadzone * -1){
        novspd_r = 0;
      }
    }

    //노브 위치값에 속도 넣기
    novro_l += novspd_l;
    novro_r += novspd_r;
  
    //노브 위치값 0~360 이내의 값이 되도록 조정
    if(novro_l > 360){
      novro_l -= 360;
    }
    else if (novro_l < 0){
      novro_l += 360;
    }
    if(novro_r > 360){
      novro_r -= 360;
    }
    else if (novro_r < 0){
      novro_r += 360;
    }


//-------------------------------------------------------------------------------// 노브 작동, 노브 LED


    if ((runningtime - novllastmovetime) >= novledofftime) {
      strip1.clear();
      strip1.show();
      novro_l = 0.0;
    }

    if ((runningtime - novrlastmovetime) >= novledofftime) {
      strip2.clear();
      strip2.show();
      novro_r = 360.00;
    }

    //노브 LED
     if (novspd_l != 0) {
    ledon1 = (stripLED0 / ledlight) - ((int)(novro_l / (360 / (double)stripLED0)) % (stripLED0 / ledlight)) - 1;
    novllastmovetime = runningtime;
    strip1.clear();
    strip1.setBrightness(255);
  
    setPosition(strip1, ledon1, strip1.Color(255,255,255), strip1.Color(0, 0, 255), ((uint32_t)0 << 16) | ((uint32_t)0 << 8) | (uint32_t)255, novspd_l < 0 ? -2 : 2); //서클
     }

    if (ledon1 == 1 || ledon1 == 13) { //360 ->  0 혹은 반대 상황일떄 LED 후처리
      strip1.setPixelColor(15, 0, 0, 0);
    } else if (ledon1 == 0) {
      strip1.setPixelColor(15, 0, 0, 255);
      strip1.setPixelColor(14, 0, 0, 0);
    }
    if (ledon1 == 14 || ledon1 == 3) {
      strip1.setPixelColor(0, 0, 0, 0);
    } else if (ledon1 == 15) {
      strip1.setPixelColor(0, 0, 0, 255);
      strip1.setPixelColor(1, 0, 0, 0);
    }
    

    if (novspd_r != 0) { 
    //R 노브쪽 LED
    ledon2 = (stripLED0 / ledlight) - ((int)(novro_r / (360 / (double)stripLED0)) % (stripLED0 / ledlight)) - 1;
    novrlastmovetime = runningtime;
    strip2.clear();
    strip2.setBrightness(255);

    setPosition(strip2, ledon2, strip2.Color(255, 255, 255), strip2.Color(255, 0, 0), ((uint32_t)255 << 16) | ((uint32_t)0 << 8) | (uint32_t)0, novspd_r < 0 ? -2 : 2); //서클
    }

    if (ledon2 == 1 || ledon2 == 13) {  //360 ->  0 혹은 반대 상황일떄 LED 후처리
      strip2.setPixelColor(15, 0, 0, 0);
    } else if (ledon2 == 0) {
      strip2.setPixelColor(15, 0, 255, 0);
      strip2.setPixelColor(14, 0, 0, 0);
    }
    if (ledon2 == 14 || ledon2 == 3) {
      strip2.setPixelColor(0, 0, 0, 0);
    } else if (ledon2 == 15) {
      strip2.setPixelColor(0, 0, 255, 0);
      strip2.setPixelColor(1, 0, 0, 0);
    }
 /*
  기본값 : L = Red / R = Green
 
  아래 코드 수정으로 색 교체 가능
  (L코드) : setPosition(strip1, ledon1, strip1.Color(움직이는 LED의 중앙 색깔), strip1.Color(빨강, 초록, 파랑), ((uint32_t)빨강 << 16) | ((uint32_t)초록 << 8) | (uint32_t)파랑, novspd_l < 0 ? -2 : 2);

  (R코드) : setPosition(strip2, ledon2, strip2.Color(움직이는 LED의 중앙 색깔), strip2.Color(빨강, 초록, 파랑), ((uint32_t)빨강 << 16) | ((uint32_t)초록 << 8) | (uint32_t)파랑, novspd_r < 0 ? -2 : 2);
  
  그 외에 후처리 색깔값도 변경해줘야 합니다. 방식은 아래 코드
  setPixelColor(위치, 빨강, 초록, 파랑)

  위치값은 건들면 안되고 RGB값만 변경해주세요
 */

//노브 입력
    if (novspd_l > 0){
        Joystick.button(8,0);
        Joystick.button(9,1);
    }
    else if (novspd_l < 0){
      Joystick.button(8,1);
      Joystick.button(9,0);
    }
    else if ( novspd_l == 0){
      if( (runningtime - novllastmovetime) > nov_button_sensitivity)
      {
        Joystick.button(8,0);
        Joystick.button(9,0);
      }
    }
      
      if (novspd_r > 0){
        Joystick.button(10,0);
        Joystick.button(11,1);
      }
      else if (novspd_r < 0){
        Joystick.button(10,1);
        Joystick.button(11,0);
      }
      else if ( novspd_r == 0){
        if( (runningtime - novrlastmovetime) > nov_button_sensitivity)
        {
          Joystick.button(10,0);
          Joystick.button(11,0);
        }
      }
    
//볼륨 관련 코드
  if (voL != 0) {
    if (voL > 0){
      Keyboard.press(KEY_MEDIA_VOLUME_INC);
      delay(50);
      Keyboard.release(KEY_MEDIA_VOLUME_INC);
    }
    if (voL < 0) {
      Keyboard.press(KEY_MEDIA_VOLUME_DEC);
      delay(50);
      Keyboard.release(KEY_MEDIA_VOLUME_DEC);
    }
  }

//즐겨찾기 코드
    if(digitalRead(FVRIT) == HIGH) {
      favrit = false;
    } else {
      favrit = true;
    }
    if (favrit == true) {
       Keyboard.press(KEYPAD_0); //0X62
       delay(50);
       Keyboard.release(KEYPAD_0);
    }
        
    //조이스틱 전송
    Joystick.send_now();
}


//-------------------------------------------------------------------------------// 
//▼ 노브 LED 설정
void setPosition(Adafruit_NeoPixel &strip, int ledon, uint32_t mainColor, uint32_t adjacentColor, uint32_t offsetColor, int offset) {
  for (int i = 0; i < ledlight; i++) {
    int pixelIndex = ledon + (i * (stripLED0 / ledlight));
    strip.setPixelColor(pixelIndex, mainColor);

    int prevIndex = (pixelIndex - 1 + strip.numPixels()) % strip.numPixels();
    int nextIndex = (pixelIndex + 1) % strip.numPixels();

    strip.setPixelColor(prevIndex, adjacentColor);
    strip.setPixelColor(nextIndex, adjacentColor);
    strip.setPixelColor(pixelIndex + offset, offsetColor);
  }
  strip1.show();
  strip2.show();
}
