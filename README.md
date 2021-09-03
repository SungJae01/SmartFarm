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

```c
//아두이노 우노 코드
#include <Arduino.h>
#include <U8g2lib.h>
#include "DHT.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define DHTPIN 2     // DHT11이 연결된 핀

#define DHTTYPE DHT22   // DHT 11, DHT시리즈중 11을 선택합니다.

DHT dht(DHTPIN, DHTTYPE);
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

String values;
int L;

void setup(){
 //Initializes the serial connection at 9600 to sent sensor data to ESP8266.
  Serial.begin(9600); 

  dht.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  
}

 
void loop() {

    //get comma indexes from values variable
    int fristCommaIndex = values.indexOf(',');
    int secondCommaIndex = values.indexOf(',', fristCommaIndex+1);
    int thirdCommaIndex = values.indexOf(',', secondCommaIndex+1);
    
    //get sensors data from values variable by  spliting by commas and put in to variables  
    String temp_value = values.substring(0, fristCommaIndex);
    String humi_value = values.substring(fristCommaIndex+1, secondCommaIndex);
    String Light_value = values.substring(secondCommaIndex+1, thirdCommaIndex);

      // get sensors data and put in to values variables as a string.
   values= (get_temp_value()+','+ get_humi_value() +','+ get_Light_value() +',');
       // removed any buffered previous serial data.
       Serial.flush();
       delay(500);
       // sent sensors data to serial (sent sensors data to ESP8266)
       Serial.print(values);
       delay(500);

    u8g2.setFont(u8g2_font_unifont_t_korean1);
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do {
      u8g2.setCursor(0, 15);
      u8g2.print("Temp : " + get_temp_value()+ " °C");
      u8g2.setCursor(0, 35);
      u8g2.print("Humi : " + get_humi_value() + " %"); 
      u8g2.setCursor(0, 55);
      u8g2.print("Light : " + get_Light_value() + " %"); 
    } while ( u8g2.nextPage());
}

String get_temp_value(){  
    float t = dht.readTemperature();// 온도를 측정합니다.
    delay(50);
    return String(t);  
}

String get_humi_value(){ 
   
    float h = dht.readHumidity();// 습도를 측정합니다.
    delay(50);
    return String(h);  
      
}

String get_Light_value(){ 
    float res;
    float L = analogRead(A0);// 조도를 측정합니다.
    int i = 1024;
    int j = 100;
    res = L*j/i;
    delay(50);
    return String(res);  
    
}
```

```c
//ESP8266-01모듈 코드
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "kimlab-21be3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "r5hljBYYrBhfysIZuiKEuQaulKtaDSYJumICdYgb"

//#define FIREBASE_HOST "test1-69dfc-default-rtdb.firebaseio.com"
//#define FIREBASE_AUTH "QOLdKEPtKAp8DcK1YBHIUo1FOobHD7EX223faEfB"
////우리집
//#define WIFI_SSID "U+Net3CA8"
//#define WIFI_PASSWORD "30AB@PFB97"
//Camp51.9
#define WIFI_SSID "KT_GiGA_ABA8"
#define WIFI_PASSWORD "7cb72ck295"

String values,sensor_data;
int n = 0;
int i = 0;
char *str[] = {"연결","연결.","연결..","연결..."};

String values_read;

void setup() {
  //Initializes the serial connection at 9600 get sensor data from arduino.
  Serial.begin(9600);
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  
  Firebase.setString("Connect","연결");
  }
  
  

void loop() {

 bool Sr = false;
 
  while(Serial.available()){
    
        //get sensor data from serial put in sensor_data
        sensor_data=Serial.readString(); 
        Sr=true;
        
    }

  if(Sr==true){
    
    values=sensor_data;
    
    //get comma indexes from values variable
    int fristCommaIndex = values.indexOf(',');
    int secondCommaIndex = values.indexOf(',', fristCommaIndex+1);
    int thirdCommaIndex = values.indexOf(',', secondCommaIndex+1);
    
    //get sensors data from values variable by  spliting by commas and put in to variables  
    String temp_value = values.substring(0, fristCommaIndex);
    String humi_value = values.substring(fristCommaIndex+1, secondCommaIndex);
    String Light_value = values.substring(secondCommaIndex+1, thirdCommaIndex);
    
    
    Firebase.getFloat("Log/Temperature");
    Firebase.getFloat("Log/Humidity");
    Firebase.getFloat("Log/Light");
    
    Firebase.setFloat("Log/number", n++);
    
    if(n == 101){
      n = 0;
    }
    
    Firebase.setString("Connect",str[i++]);
    if(i == 4){
      i = 0;
    }
    
    delay(300);
    
    if (Firebase.failed()) {
        return;
    }
  
  }   
}
```

hackster.io의 [[Send multiple sensors data to Firebase using ESP8266]](https://www.hackster.io/pulasthi-nanayakkara/send-multiple-sensors-data-to-firebase-using-esp8266-f2f38b)를 참고하였습니다.

![파이어베이스](https://user-images.githubusercontent.com/88194064/131805681-67a60855-7dde-4f15-b286-280d71c00866.png) 센서 값 저장 모습

### 안드로이드 스튜디오
 스마트팜의 상태를 실시간으로 모니터링 할 수 있는 어플 제작중..    

 와이파이 통신으로 데이터베이스에 저장한 센서값을 값을 읽어 어플에 표시

<img src="사진\어플.jpg" align="left">

 

