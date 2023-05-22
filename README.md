# SDVX_Teensy2.0_controller

이 코드는 Teensy 2.0을 기반으로 제작한 SDVX 컨트롤러를 제작하는데 사용됩니다.

이 코드는 아카라이브 리듬게임 채널 유저 [코듀](https://arca.live/b/mgamm/68743390) 님의 글에 첨부된 코드를 기반으로 하였으며
기능 추가 및 삭제, 최적화 작업을 거쳤습니다.


해당 코드는 CC라이센스에 기반해 BY-SC-NA 가 적용되며
원작자의 라이센스에 따라 해당 코드를 이용한 수익 창출은 불가합니다.

부품 정보는 [구글 시트](https://docs.google.com/spreadsheets/d/19fmriJb6eNJEz0nDAXWIfqVCtt-jTtWN05BttMs27ew/edit#gid=0)에서 확인 가능합니다

# 코드의 변경점 및 수정 사항

## 제거 및 변경사항 ##
- EEFROM을 이용한 설정 시스템이 제거되었습니다.
- 배열화를 사용하여 코드 일부분을 최적화 하였습니다.

## 추가된 사항 ##
- 클릭형 엔코더 관련 구문을 추가하여 볼륨 컨트롤과 즐겨찾기(키패드 0) 버튼을 사용 할 수 있습니다.
- LED 버젼에 한정하여 추가 LED를 기판에 설치 할 수 있습니다.


# 커스터마이징 #

## PINMAP ##
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
기본 설정 값입니다. 첨부된 PCB를 주문 제작해서 이용하신다면 딱히 수정할 사항은 없습니다.




## LED ##

LED의 색깔을 변경하기 위해서 아래 과정을 거쳐야 합니다

```arduino
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

해당 코드에서 SetPosition로 전달되는 함수값과, 그 아래 후처리 코드를 수정하면 됩니다.

만약 색깔을 변경하고 싶으시다면, 아래 코드에서 빨강, 초록, 파랑에 0~255값을 넣어주시면 됩니다.
    
```arduino
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

## Strip LED ##

**작성중**

왼쪽 엔코더 13              두 LED를 묶어서 stripLED0로 선언
오른쪽 엔코더 13            
가운데 스트랩 LED 36        stripLED1로 선언
각 버튼 LED 22개            stripLED2로 선언


만약 개수를 변경하고 싶을땐 아래 변수를 고려해야 합니다


1. stripLED0는 고정된 개수라 변경할 필요 x 만약 변경하더라도 자유롭게 가능


2. stripLED1은 길이가 달라질 시 스터트 버튼 중간점이 변화할 수 있음. 만약 길이가 달라진다면, 145번쨰 줄과 154번째 줄에 있는 for(int i = 13; i <= 22; i++) 구문을 수정해줘야 합니다

필자의 세팅은 13+10+13 이라 중앙부터 양옆으로 5칸씩 13~22로 총 10개 값으로 설정했습니다

3. StripLED2은 길이가 달라질 시 설정한 배열을 수정해야 합니다

36~38번째 줄에 있는 버튼 LED의 배열을 각 LED개수에 맞게 수정해야 합니다

차례대로 BT-4 , BT-3 , BT-2 , BT-1 , FX-L , FX-R 순서이므로 잘 보고 써야합니다.

참고로 필자의 세팅은 4, 4, 4, 4, 3, 3입니다.


