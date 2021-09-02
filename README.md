# ICT 멘토링 한이음 공모전

**2021 수원대학교 정보통신학부 김시현 교수님 랩**

🤱**랩장** : 최나영

👪**랩원** : 박성재, 석민경, 이나영

## 신선한 채소를 매일~ 나만의 스마트 팜 [2021-04-13 ~]

 2021년 1학기를 시작하며 랩원들과 한이음 프로젝트를 진행하기로 했습니다. 주제는 개인 가정용 스마트팜 제작입니다. 농약 걱정 없는 신선한 채소를 가정이나, 자취방에서 직접 키워 먹을 수 있는 가정용/개인용 스마트 팜을 소형의 크기로 제작하려고 합니다.

### 구성도

- 토양의 습도를 측정하여 워터펌프로 토양에 적절한 양의 물을 공급
- 식물의 종류를 파악하고 식물 성장에 적합한 온도를 제공
- 식물에게 적한 조도를 파악하여 RGB LED에 비춰지는 조도의 양을 조절 
- DB에는 아두이노 센서로부터 보내온 실시간 환경 데이터인 온습도, 조도, 이산화탄소 등의 센서 값이 적재
- APP을 통해 실시간 모니터링,제어

### 주요 기능

- 개인용 스마트 팜
- 모바일 기기를 이용한 스마트 팜 원격제어
- 토양의 온도, 습도를 감지하여 환기팬, 급수 모터 동작
- 식물의 최적 생장을 위한 LED 조명 제어
- 워터펌프, 환풍 팬, DC motor, REG LED 구동을 통해 최적 생육환경 조성

| **구분** | **기능**                                                     | **설명**                                                     |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| S/W      | Bluetooth                                                    | 모바일 스마트 농장 센서 데이터, 액츄에이터 동작 및 상태, 카메라 영상, 이벤트 수신 및 저장 |
| APP      | 안드로이드 스튜디오를 이용하여 앱 제작                       |                                                              |
| H/W      | 온도, 습도 센서                                              | 토양의 온도, 습도를 감지해 워터펌프 가동                     |
| 컨트롤러 | 아두이노 보드를 이용해 센서 신호 처리,   엑츄에이터 구동 및 무선통신 컨트롤러로 전송 |                                                              |

 

  ㅇ S/W 주요 기능

| **기능**                     | **설명**                                                     |
| ---------------------------- | ------------------------------------------------------------ |
| 스마트팜 환경 모니터링       | 아두이노에 있는 온도, 토양 습도, 조도  값을 스마트폰에 제공하여 사용자가 모니터링 할 수 있도록 함 |
| 사용자가 스마트팜  환경 조절 | 온도, 토양 습도, 조도 값이 식물 적정  값에 맞도록 각 센서를 작동 시켜서 조절할 수 있게 함 |
| 데이터베이스에 센서 값 입력  | 아두이노 우노에서 ESP8266-01을 사용하여 WiFi 통신으로 데이터베이스에 센서 값 저장 |

 

  ㅇ H/W 주요 기능

| **기능/부품**    | **설명**                                                     |
| ---------------- | ------------------------------------------------------------ |
| 온습도 센서      | 스마트 팜의 온습도를 측정하여 값에 맞는 동작을 수행하게 함   |
| LED              | 조도 센서를 통해 측정한 값으로 led의 밝기를 조절             |
| 조도 센서        | 스마트 팜의 조도를 측정하여 식물에게 최적의 환경을 제공      |
| 이산화 탄소 센서 | 스마트 팜의 이산화 탄소 농도를 측정                          |
| 토양 수분 센서   | 토양에 수분함량을 측정하여 식물에 필요한 수분을 알 수 있게 함 |

### 아두이노 Arduino

 스마트팜 내부의 데이터 값을 데이터베이스로 저장하려한다. 아두이노 우노의 센서값을 ESP8266-01을 사용하여 WiFi 통신하여 데이터베이스에 저장한다.
 
 [아두이노 우노 코드](Arduino/Arduino.ino) 바로가기   
 [ESP8266-01 코드](ESP8266/ESP8266.ino) 바로가기

hackster.io의 [[Send multiple sensors data to Firebase using ESP8266]](https://www.hackster.io/pulasthi-nanayakkara/send-multiple-sensors-data-to-firebase-using-esp8266-f2f38b)를 참고하였습니다.

<img src="사진\파이어베이스.png" align="left"> ««« **센서 값 저장 모습**

