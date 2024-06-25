# ICT 멘토링 한이음 공모전

**2021 수원대학교 정보통신학부 김시현 교수님 랩**

🤱**랩장** : 최나영[19]

👪**랩원** : 박성재[20], 석민경[19], 이나영[19]

## 신선한 채소를 매일~ 나만의 스마트 팜 [2021-04-13 ~ 2021-12-10]

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
| S/W      | WiFi                                                         | 모바일 스마트 농장 센서 데이터, 액츄에이터 동작 및 상태, 이벤트 수신 및 저장 |
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

| **기능/부품**  | **설명**                                                     |
| -------------- | ------------------------------------------------------------ |
| 온습도 센서    | 스마트 팜의 온습도를 측정하여 값에 맞는 동작을 수행하게 함   |
| LED            | 조도 센서를 통해 측정한 값으로 led의 밝기를 조절             |
| 조도 센서      | 스마트 팜의 조도를 측정하여 식물에게 최적의 환경을 제공      |
| 토양 수분 센서 | 토양에 수분함량을 측정하여 식물에 필요한 수분을 알 수 있게 함 |
| 초음파 센서    | 물통의 물의 양을 나타내어 모니터링 할 수 있게 함             |
| OLED 모니터    | 스마트팜 기기에 모니터를 통해 스마트팜 내부 환경을 알 수 있게 함 |

### 아두이노 Arduino

#### 코드

 스마트팜 내부의 데이터 값을 데이터베이스로 저장하려한다. 아두이노 우노의 센서값을 ESP8266-01을 사용하여 WiFi 통신하여 데이터베이스에 저장한다.

 [NodeMCU 코드](NodeMCU/nodeMCU.ino) 바로가기  
 [아두이노 메가 WiFi 코드](ESP8266/ESP8266.ino) 바로가기

hackster.io의 [[Send multiple sensors data to Firebase using ESP8266]](https://www.hackster.io/pulasthi-nanayakkara/send-multiple-sensors-data-to-firebase-using-esp8266-f2f38b)를 참고하였습니다.

#### 순서도

<img src="https://user-images.githubusercontent.com/88194064/143600220-6dbfb997-43cb-4f71-99c1-698a3730b787.png" alt="순서도" style="float:left; zoom:100%;" />



#### 기본 센서 및 엑츄에이터 회로도

 아래 회로도는 스마트팜에 들어가는 센서 & 엑츄에이터이다. 하지만 스마트팜의 들어가는 센서 & 엑추에이터는 여러개를 사용하여야 한다. 하지만 아두이노 우노의 핀 개수는 아날로그핀 6개 디지털핀 13개로 스마트팜을 구현하기에 핀 개수가 부족하다.<img src="https://user-images.githubusercontent.com/88194064/132123562-54bd586a-bdab-43ac-8cf1-2b41392820c8.jpg" style="zoom:25%;" >



##### 74HC595 시프트 레지스터

 **시프트 레지스터 (shift register)** :  [디지털 회로](https://ko.wikipedia.org/wiki/디지털_회로)에서 선형 방식으로 설치된 [프로세서 레지스터](https://ko.wikipedia.org/wiki/프로세서_레지스터)의 집합이며, 회로가 활성화 되었을 때 데이터를 줄 아래로 이동시키는 것과 같은 방법으로 입출력을 서로 연결하고 있다.

 아두이노 우노의 핀 개수를 늘리고자 아두이노 핀 3개를 사용하여 디지털핀을 8개까지 늘릴 수 있는 74HC595 시프트 레지스터를 사용하기 위해 [Tinkercad](https://www.tinkercad.com/)를 사용하여 공부해 보았다.![74HC595 레지스터](https://user-images.githubusercontent.com/88194064/135709140-29308774-1847-4f1c-8253-161fcff635ac.png)

사용법은 [이 영상](https://www.youtube.com/watch?v=LJrWb9RuHdE) 을 참고하였다. 

##### NodeMCU ESP8266

 아두이노의 핀 개수 문제와 와이파이 통신의 편리성을 위해 NodeMCU, 아두이노 우노 & ESP8266, 74HC595 시프트 레지스터 1개를 사용하기로 결정하였다. 시프트 레지스터는 모터 드라이브 1개를 제어하는데 사용된다.

<img src="https://user-images.githubusercontent.com/88194064/140882432-44c77994-556e-4ec0-bc91-de61be04daee.png" alt="ESP8266-NodeMCU" style="float:left; zoom:50%;" /><img src="https://user-images.githubusercontent.com/88194064/143594188-0823584f-e8b9-4d7e-950c-54e38ca01acc.png" alt="ESP8266" style="float:left; zoom:50%;" /><img src="https://user-images.githubusercontent.com/88194064/143594204-e5ad95f3-fcb8-4423-8a69-dbf673d48bad.png" alt="ESP8266-NodeMCU" style="float:left; zoom:70%;" />

















   ㅇ NodeMCU H/W 센서 & 엑추에이터 핀

| 이름                    | 개수 | Analog&Digital 핀 번호                          |
| ----------------------- | ---- | ----------------------------------------------- |
| 74HC595 시프트 레지스터 | 1    | DATA_PIN : D5 , LATCH_PIN : D6 , CLOCK_PIN : D7 |
| 릴레이                  | 1    | D6                                              |
| LED바                   | 1    |                                                 |
| OLED                    | 1    | D1, D2                                          |

   ㅇ 아두이노 우노 H/W 센서 & 엑추에이터 핀

| 이름           | 개수 | Analog&Digital 핀 번호 |
| -------------- | ---- | ---------------------- |
| 토양 수분 센서 | 2    | A0, A1                 |
| 초음파 센서    | 1    | D7, D8                 |
| ESP8266        | 1    | RX, TX                 |

#### 최종 회로도

![회로도2](https://user-images.githubusercontent.com/88194064/143608698-d4c527c8-9b00-48b7-9de7-ecccf129e549.png)



### 안드로이드 스튜디오

 스마트팜의 상태를 실시간으로 모니터링 할 수 있는 어플 제작중...

![어플](https://user-images.githubusercontent.com/88194064/132038237-3c9280ab-3bf8-4c52-974f-b8e4689b0f63.jpg)

와이파이 통신으로 데이터베이스에 저장한 센서값을 값을 읽어 어플에 표시



### 3D 모델링

 전체적인 모습

<img src="https://user-images.githubusercontent.com/88194064/142598956-96bb56b6-d40c-444d-9ea4-1ed92c82692c.png" alt="모델링" style="zoom:100%; float:left" />

<img src="https://user-images.githubusercontent.com/88194064/142597355-dc569fd0-e3b8-4e3e-a698-ee6ede2920dd.gif" alt="모델링" style="zoom:100%; float:left" />





### 프로젝트 결과

 대실패

 실패 요인
 1. 프로젝트 주제 너무 많은 기능을 추가하려하였다.
 2. 스마트팜에서 작물을 기계가 전부 관리하게 만들고싶은 욕심이 컷다.
 3. 아두이노 인터럽트기능을 잘 활용하지 못하였다
 4. 프로젝트 마간기한을 잘 맞추지 못했고 여러 기능들은 구현했지만 너무 허술하였다.

### 2024년 다른 주제로 프로젝트 재시도 후 성공

**[2024.05.10 ~ 2024.06.11]**

https://github.com/SungJae01/IoT_FinalProject.git
