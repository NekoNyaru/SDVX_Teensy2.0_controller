# SDVX_Teensy2.0_controller
![initial](https://github.com/NekoNyaru/SDVX_Teensy2.0_controller/assets/113786183/fc266055-f89b-4206-9b4f-583e2618ad23)
(Controller LED Build)


이 코드는 Teensy 2.0을 기반으로 제작한 SDVX 컨트롤러를 제작하는데 사용됩니다.

이 코드는 아카라이브 리듬게임 채널 유저 [코듀](https://arca.live/b/mgamm/68743390) 님의 글에 첨부된 코드를 기반으로 하였으며
기능 추가 및 삭제, 최적화 작업을 거쳤습니다.


해당 코드는 CC BY-NC-SA 2.0 KR의 라이선스를 적용하며

원작자의 라이선스에 따라 해당 코드를 이용한 수익 창출은 불가합니다.

부품 정보는 [Google Spreadsheet](https://docs.google.com/spreadsheets/d/19fmriJb6eNJEz0nDAXWIfqVCtt-jTtWN05BttMs27ew/edit#gid=0)에서 확인 가능합니다

# 사용법
해당 [Google Drive](https://drive.google.com/drive/folders/1MX582bQeySP7_dZotP_uXt4V2uPrYggn?usp=sharing)에 `Teensy 관련 유틸리티` 폴더에 작성일 기준 최신 버전인 Arduino IDE 2.1.0과 Teensy Loader를 업로드했습니다.

폴더에 함께 동봉되어 있는 가이드를 따라 Arduino에서 Teensy보드 라이브러리를 인스톨 한 후에, Teensy2.0 보드를 컴퓨터에 연결하고 Github에서 사용할 버전을 다운로드 받아 업로드하세요.

* * *
### 코드의 변경점 및 수정 사항

##### 제거 및 변경사항 ##
- EEFROM을 이용한 설정 시스템이 제거되었습니다.
- 배열화를 사용하여 코드 일부분을 최적화 하였습니다.

##### 추가된 사항 ##
- 클릭형 엔코더 관련 구문을 추가하여 볼륨 컨트롤과 즐겨찾기(키패드 0) 버튼을 사용 할 수 있습니다.
- LED 버젼에 한정하여 추가 LED를 기판에 설치 할 수 있습니다.

* * *
### 코드 상세 설명 / 커스터마이징 #

##### PINMAP
```
NAME    BTN     LED
BT-1    1       19
BT-2    18      2
BT-3    24      4
BT-4    16      17

FX-L    12      13
FX-R    20      21

Start   0       1

NAME    LEFT    RIGHT
Knov-R  5       6  
Knov-L  7       8
```
버튼 입력에 관한 기본 설정 값입니다. 첨부된 PCB를 주문 제작해서 이용하신다면 딱히 수정할 사항은 없습니다.



##### LED 색깔 변경

LED의 색깔을 변경하기 위해서 아래 과정을 거쳐야 합니다

```c++
  if (novspd_l != 0) {
    clearL = false;
    //L 노브쪽 LED
    ledon1 = (stripLED0 / ledlight) - ((int)(novro_l / (360 / (double)stripLED0)) % (stripLED0 / ledlight)) - 1;
    //스타트버튼 LED
    ledon3 = (stripLED1 / ledlight) - ((int)(novro_l / (360 / (double)stripLED1)) % (stripLED1 / ledlight)) - 1;
    novllastmovetime = runningtime;
    strip1.clear();
    strip1.setBrightness(255);
  
    setPosition(strip1, ledon1, strip1.Color(255,255,255), strip1.Color(0, 0, 255), ((uint32_t)0 << 16) | ((uint32_t)0 << 8) | (uint32_t)255, novspd_l < 0 ? -2 : 2); //서클
    setPositionS(strip3, ledon3, strip1.Color(255,255,255), strip3.Color(0, 0, 255)); //스트립
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
    clearR = false;
    //R 노브쪽 LED
    ledon2 = (stripLED0 / ledlight) - ((int)(novro_r / (360 / (double)stripLED0)) % (stripLED0 / ledlight)) - 1;
    //스타트버튼 LED
    ledon4 = (stripLED1 / ledlight) - ((int)(novro_r / (360 / (double)stripLED1)) % (stripLED1 / ledlight)) - 1;
    novrlastmovetime = runningtime;
    strip2.clear();
    strip2.setBrightness(255);

    setPosition(strip2, ledon2, strip2.Color(255, 255, 255), strip2.Color(0, 255, 0), ((uint32_t)0 << 16) | ((uint32_t)255 << 8) | (uint32_t)0, novspd_r < 0 ? -2 : 2); //서클
    setPositionS(strip3, ledon4, strip2.Color(255, 255, 255), strip3.Color(255, 0, 0)); //스트립
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
```

해당 코드에서 `SetPosition`으로 전달되는 함수값과, 그 아래 `후처리 코드`를 수정하면 됩니다.

만약 색깔을 변경하고 싶으시다면, 아래 코드에서 빨강, 초록, 파랑에 0~255값을 넣어주시면 됩니다.
해당 구문은 코드 안에 주석으로도 첨부해뒀으니 확인해주세요
    
```c++
    setPosition(strip2, ledon2, strip2.Color(255, 255, 255), strip2.Color(빨강, 초록, 파랑), ((uint32_t)빨강 << 16) | ((uint32_t)초록 << 8) | (uint32_t)파랑, novspd_r < 0 ? -2 : 2); //서클
    setPositionS(strip3, ledon4, strip2.Color(255, 255, 255), strip3.Color(초록, 빨강, 파랑)); //스트립
    }

    if (ledon2 == 1 || ledon2 == 13) {  //360 ->  0 혹은 반대 상황일떄 LED 후처리
      strip2.setPixelColor(15, 0, 0, 0);
    } else if (ledon2 == 0) {
      strip2.setPixelColor(15, 빨강, 초록, 파랑);
      strip2.setPixelColor(14, 0, 0, 0);
    }
    if (ledon2 == 14 || ledon2 == 3) {
      strip2.setPixelColor(0, 0, 0, 0);
    } else if (ledon2 == 15) {
      strip2.setPixelColor(0, 빨강, 초록, 파랑);
      strip2.setPixelColor(1, 0, 0, 0);
    }
```

### Strip LED / LED Mode Only
이하는 LED모델을 제작하려는 분들을 위한 커스터마이징 가이드입니다.
개인적으로 LED를 추가하거나 제외하고 싶을 때 아래 가이드라인을 따라주세요

우선 현재 코드에 세팅된 값은 아래와 같습니다 

```
왼쪽 엔코더 LED         | stripLED0 13              두 LED를 묶어서 stripLED0로 선언
오른쪽 엔코더 LED       | stripLED0 13
가운데 스트랩 LED       | stripLED1 36              stripLED1로 선언
버튼 LED 22개          | stripLED2 22              stripLED2로 선언
```

만약 LED의 개수를 변경하고 싶을땐 아래 변수를 고려해야 합니다

##### stripLED0
엔코더에 설치될 LED를 제어합니다
시트에 포함된 LED를 사용한다면 변경하지 않으셔도 됩니다

##### stripLED1
기판 상단부에 설치될 LED를 제어합니다

제가 제작한 기판같이 세팅해도 상관은 없으나, 본인의 의도에 따라 얼마든지 레이아웃이나 개수는 수정될 수 있습니다.

stripLED1은 길이가 달라질 시 LED의 중앙값이 변할 수 있습니다.
만약 길이가 달라진다면 우선 `부품` 에서 `#define stripLED1 36` 값을 설치할 LED개수에 맞게 수정하셔야 합니다. 

그 후에
```c++
        if (b_stat[0] == true) { // START
      Joystick.button(1, 1);
      digitalWrite(le[0], HIGH);
      Keyboard.releaseAll();
      voL = 0;
      for (int i = 13; i <= 22; i++) {                   // 버튼 좌우로 파란색 LED 밝히는 구문
        strip3.setBrightness(255); 
        strip3.setPixelColor(i, 0, 0, 255);             //
      }                                                 //
      strip3.show();                                    //
```
Start 버튼 코드에서 `for(int i = 13; i <= 22; i++)` 구문을 수정해줘야 합니다

필자의 세팅은 13+10+13 이라 중앙부터 양옆으로 5칸씩 13~22로 총 10개 값으로 설정했습니다

* * *
##### stripLED2
각 버튼 위에 설치 될 LED를 제어합니다.
채널방식을 사용해서 위치값만 지정해주면 알아서 작동합니다.

우선 `부품` 에서 `#define stripLED2 22` 값을 적용할 LED의 총 개수로 수정해주세요

그리고 `버튼 LED`의 배열을 각 LED개수에 맞게 수정해야 합니다

```c++
  // 버튼 LED
  int BTN_S[] = {12, 8, 4, 0, 16, 19};              // 각 버튼을 누를때마다 반응하는 스트립 LED 의 위치 지정 (시작점)
  int BTN_E[] = {15, 11, 7, 3, 18, 21};             // 끝점
```

차례대로 BT-4 , BT-3 , BT-2 , BT-1 , FX-L , FX-R 순서이므로 구분해서 작성해야 합니다

필자의 세팅은 4, 4, 4, 4, 3, 3입니다.



