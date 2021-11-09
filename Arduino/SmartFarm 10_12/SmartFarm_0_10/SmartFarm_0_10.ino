#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "DHT.h"

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Set these to run example.
//#define FIREBASE_HOST "kimlab-21be3-default-rtdb.firebaseio.com"
//#define FIREBASE_AUTH "r5hljBYYrBhfysIZuiKEuQaulKtaDSYJumICdYgb"

#define FIREBASE_HOST "test1-69dfc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "QOLdKEPtKAp8DcK1YBHIUo1FOobHD7EX223faEfB"
//우리집
//#define WIFI_SSID "U+Net3CA8"
//#define WIFI_PASSWORD "30AB@PFB97"
//Camp51.9
#define WIFI_SSID "KT_GiGA_ABA8"
#define WIFI_PASSWORD "7cb72ck295"

#define BUTTON_PIN 2

//LED 바
#define PIN 2              // 디지털 입력 핀 설정
#define NUMPIXELS 15        // Neopixel LED 소자 수 (LED가 24개라면 , 24로 작성)
#define BRIGHTNESS 255     // 밝기 설정 0(어둡게) ~ 255(밝게) 까지 임의로 설정 가능
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//온습도 센서 DHT22
#define DHTPIN 0 //D1
#define DHTTYPE DHT22

//토양 습도 센서 x4
#define MOISTURE_1 A0
//#define MOISTURE_2 A2
//#define MOISTURE_3 A3
//#define MOISTURE_4 A4

//모터드라이버 1
#define WATERPUMP1_ENA 22   //D22
#define WATERPUMP1_EN1 23   //D23
#define WATERPUMP1_EN2 24   //D24

#define WATERPUMP2_ENB 25   //D25
#define WATERPUMP2_EN3 26   //D26
#define WATERPUMP2_EN4 27   //D27

int WaterPump1_Speed_A=0;
int WaterPump2_Speed_B=0;


//모터드라이버 2
#define WATERPUMP3_ENA 28   //D28
#define WATERPUMP3_EN1 29   //D29
#define WATERPUMP3_EN2 30   //D30

#define WATERPUMP4_ENB 31   //D31
#define WATERPUMP4_EN3 32   //D32
#define WATERPUMP4_EN4 33   //D33

int WaterPump3_Speed_A=0;
int WaterPump4_Speed_B=0;

//모터드라이버 3
#define FAN1_ENA 34   //D34
#define FAN1_EN1 35   //D35
#define FAN1_EN2 36   //D36

#define FAN2_ENB 37   //D37
#define FAN2_EN3 38   //D38
#define FAN2_EN4 39   //D39

int Fan1_Speed_A=0;
int Fan2_Speed_B=0;


//릴레이 x ?
#define Relay1 2
#define Relay2 3

//LED 바 x 2
#define LED1 4
#define LED2 5

DHT dht(DHTPIN, DHTTYPE);
//float now_temp=0 , temp;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //LED바
  pixels.setBrightness(BRIGHTNESS);    //  BRIGHTNESS 만큼 밝기 설정 
  pixels.begin();                      //  Neopixel 제어를 시작
  pixels.show();                       //  Neopixel 동작 초기화 합니다
  
  //OLED
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();

  // 워터펌프 1, 2 제어핀 출력 설정
  pinMode(WATERPUMP1_ENA,OUTPUT);
  pinMode(WATERPUMP2_ENB,OUTPUT);
  
  // 워터펌프 1, 2 방향 제어핀 출력 설정
  pinMode(WATERPUMP1_EN1,OUTPUT);
  pinMode(WATERPUMP1_EN2,OUTPUT);
  pinMode(WATERPUMP2_EN3,OUTPUT);
  pinMode(WATERPUMP2_EN4,OUTPUT);

  // 워터펌프 3, 4 제어핀 출력 설정
  pinMode(WATERPUMP3_ENA,OUTPUT);
  pinMode(WATERPUMP4_ENB,OUTPUT);
  
  // 워터펌프 3, 4 방향 제어핀 출력 설정
  pinMode(WATERPUMP3_EN1,OUTPUT);
  pinMode(WATERPUMP3_EN2,OUTPUT);
  pinMode(WATERPUMP4_EN3,OUTPUT);
  pinMode(WATERPUMP4_EN4,OUTPUT);

  // 펜 1, 2 제어핀 출력 설정
  pinMode(FAN1_ENA,OUTPUT);
  pinMode(FAN2_ENB,OUTPUT);
  
  // 펜 1, 2 방향 제어핀 출력 설정
  pinMode(FAN1_EN1,OUTPUT);
  pinMode(FAN1_EN2,OUTPUT);
  pinMode(FAN2_EN3,OUTPUT);
  pinMode(FAN2_EN4,OUTPUT);

//  pinMode(Relay, OUTPUT);
//  pinMode(Switch,INPUT_PULLUP);
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_streamline_all_t); 
    u8g2.drawGlyph(29,43, 0x00df);
    u8g2.setFont(u8g2_font_streamline_all_t); 
    u8g2.drawGlyph(54,43, 0x00df);
    u8g2.setFont(u8g2_font_streamline_all_t); 
    u8g2.drawGlyph(79,43, 0x00df);
  } while ( u8g2.nextPage() );
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("Connect","연결");
  Serial.println("WiFi 연결 완료, Firebase 연결 완료");
  Serial.println("-------------------------------------");
  Serial.println("Device Started");
  Serial.println("-------------------------------------");

  u8g2.clearBuffer(); 
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do {
        
  } while ( u8g2.nextPage() );
}
bool oldState = HIGH;
int showType = 0;
int k = 1;
char input = 0;
//float temp_value=0, temp;
//temp=temp_value;

int timeSinceLastRead = 0;
int StateData[3];     // 1층 식물 조도, 최고온도, 최저온도
int ON_OFF;
String select;

void loop() {
  
    OLED();
    DataSetup();

    for(int i = 0; i<3; i++){
      Serial.print(i);
      Serial.print(" : ");
      Serial.println(StateData[i]);
    }
    
    float temp_value = dht.readTemperature();
//    if(temp_value!=temp){
//       //
//       temp=temp_value;
//       //데이터 베이스에 floor1에 temp값 보냄
//       Firebase.setFloat("Floor1/Temperature",temp_value);
//    }
    float humi_value = dht.readHumidity();
    int Moisture1_value = analogRead(MOISTURE_1);
//    int Moisture2_value = analogRead(MOISTURE_2);
//    int Moisture3_value = analogRead(MOISTURE_3);
//    int Moisture4_value = analogRead(MOISTURE_4);
    
    Serial.print("온도 : ");
    Serial.print(temp_value);
    delay(100);
    Serial.println("°C");
    Serial.print("습도 : ");
    Serial.print(humi_value);
    delay(100);
    Serial.println("%");
    Serial.print("토양 습도 : ");
    Serial.print(get_Moisture_value());
    delay(100);
    Serial.println("%");
//    Serial.print(Firebase.getFloat("Floor1/Moisture2"));
//    Serial.print(Firebase.getFloat("Floor1/Moisture3"));
//    Serial.println(Firebase.getFloat("Floor1/Moisture4"));
    Serial.println("-------------------------------------┐");

    DataToFirebase(temp_value, humi_value, get_Moisture_value());

//    Water_Pump(Moisture1_value,Moisture2_value,Moisture3_value,Moisture4_value);

//  pixels.setPixelColor(NUMPIXELS, pixels.Color(255,0,0));
//  pixels.show();
  delay(100);
}

float get_Moisture_value(){
    float res;
    float M = analogRead(MOISTURE_1);   //토양 습도를 측정합니다.

    int i = 1023;
    int j = 100;
    res = M*j/i;                //%로 변환
    return res;  
}

//void Water_Pump(float Moisture1_value,float Moisture2_value,float Moisture3_value,float Moisture4_value){
//  //워터펌프 1
//  if(Moisture1_value < 40){
//      digitalWrite(WATERPUMP1_EN1, LOW);   // 워터 펌프 작동
//      digitalWrite(WATERPUMP1_EN2, HIGH);
//      analogWrite(WATERPUMP1_ENA, WaterPump1_Speed_A);
//      delay(5000);
//      digitalWrite(WATERPUMP1_EN1, LOW);   // 워터 펌프 중지
//      digitalWrite(WATERPUMP1_EN2, LOW);
//      delay(2000);
//   }
//   else{
//     digitalWrite(WATERPUMP1_EN1, LOW);   // 워터 펌프 중지
//     digitalWrite(WATERPUMP1_EN2, LOW);
//   }
//   //워터펌프 2
//   if(Moisture2_value < 80){
//      digitalWrite(WATERPUMP2_EN1, LOW);   // 워터 펌프 작동
//      digitalWrite(WATERPUMP2_EN2, HIGH);
//      analogWrite(WATERPUMP2_ENA, WaterPump2_Speed_B);
//      delay(5000);
//      digitalWrite(WATERPUMP2_EN1, LOW);   // 워터 펌프 중지
//      digitalWrite(WATERPUMP2_EN2, LOW);
//      delay(2000);
//   }
//   else{
//     digitalWrite(WATERPUMP2_EN1, LOW);   // 워터 펌프 중지
//     digitalWrite(WATERPUMP2_EN2, LOW);
//   }
//   //워터펌프 3
//   if(Moisture3_value < 80){
//      digitalWrite(WATERPUMP3_EN1, LOW);   // 워터 펌프 작동
//      digitalWrite(WATERPUMP3_EN2, HIGH);
//      analogWrite(WATERPUMP3_ENA, WaterPump3_Speed_A);
//      delay(5000);
//      digitalWrite(WATERPUMP3_EN1, LOW);   // 워터 펌프 중지
//      digitalWrite(WATERPUMP3_EN2, LOW);
//      delay(2000);
//   }
//   else{
//     digitalWrite(WATERPUMP3_EN1, LOW);   // 워터 펌프 중지
//     digitalWrite(WATERPUMP3_EN2, LOW);
//   }
//   //워터펌프 4
//   if(Moisture4_value < 80){
//      digitalWrite(WATERPUMP4_EN1, LOW);   // 워터 펌프 작동
//      digitalWrite(WATERPUMP4_EN2, HIGH);
//      analogWrite(WATERPUMP4_ENA, WaterPump4_Speed_B);
//      delay(5000);
//      digitalWrite(WATERPUMP4_EN1, LOW);   // 워터 펌프 중지
//      digitalWrite(WATERPUMP4_EN2, LOW);
//      delay(2000);
//   }
//   else{
//     digitalWrite(WATERPUMP4_EN1, LOW);   // 워터 펌프 중지
//     digitalWrite(WATERPUMP4_EN2, LOW);
//   }
//}

//void Moter(float temp_value){
//  if(StateData[1]>=temp_value)&&(StateData[2]<=temp_value){
//    //모터 작동 중지
//  }else{
//    if(StateData[1]<temp_value){
//      //펜 작동
//    }
//    else if(StateData[2]>temp_value){
//      //히터 작동
//    }
//  }
//}
//
//void LED(String select){
//  
//
//}

void OLED(){
  u8g2.clearBuffer();
  if(Firebase.getFloat("Floor1/start") == 2){
      u8g2.clearBuffer();
      delay(500);
      u8g2.setFont(u8g2_font_unifont_t_korean1);  
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(9, 37);
        u8g2.print("식물 심는중");
      } while ( u8g2.nextPage() );
      delay(500);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_unifont_t_korean1);  
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(9, 37);
        u8g2.print("식물 심는중.");    // Korean "Hello World"
      } while ( u8g2.nextPage() );
      delay(500);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_unifont_t_korean1);  
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(9, 37);
        u8g2.print("식물 심는중..");    // Korean "Hello World"
      } while ( u8g2.nextPage() );
      delay(500);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_unifont_t_korean1);  
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(9, 37);
        u8g2.print("식물 심는중...");    // Korean "Hello World"
      } while ( u8g2.nextPage() );
    }
    if(Firebase.getFloat("Floor1/start") == 1) {
      u8g2.clearBuffer(); 
      u8g2.firstPage();
      do {
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_streamline_all_t); 
        u8g2.drawGlyph(54,46, 0x00db);
        u8g2.drawGlyph(54,37, 0x00d7);
        u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
        u8g2.drawGlyph(119, 9, 0x00f7);
        
        if(Firebase.getBool("Floor1/HeatingFan") == true){
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(1, 9, 0x0103);
        }
        else{
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(1, 9, 0x011f);
        }
        
        if(Firebase.getBool("Floor1/CoolingFan") == true){
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(10, 9, 0x0048);
        }
        else if(Firebase.getBool("Floor1/CoolingFan") == false){
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(10, 9, 0x0053);
        }

        if(Firebase.getBool("Floor1/WaterPump") == true){
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(19, 9, 0x0098);
        }
        else{
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(19, 9, 0x011f);
        }
      } while ( u8g2.nextPage() );
    }
    if(Firebase.getFloat("Floor1/start") == 0) {
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_ncenB10_tr); 
        u8g2.drawStr(33, 37, "KimLab");
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
        u8g2.drawGlyph(119, 9, 0x00f7);
      } while ( u8g2.nextPage() );
    }
    delay(500);
}

void DataSetup(){
    
    ON_OFF = Firebase.getInt("Floor1/start");
    Serial.println(ON_OFF);

    if(ON_OFF == 1){   // 모니터링 작동 시작
      select = Firebase.getString("Floor1/select");
      Serial.println(select);
      if(select == "사용자 설정"){
        //사용자 설정값
      }
      else{
        //고정값
        StateData[0] = Firebase.getFloat(select + "/Light");
        StateData[1] = Firebase.getFloat(select + "/Temperature_HIGH");
        StateData[2] = Firebase.getFloat(select + "/Temperature_LOW");
      }
    }else{     //작동 중지
      select = "NULL";
      StateData[0] = 0;
      StateData[1] = 0;
      StateData[2] = 0;
    }
    Serial.read();
    delay(1000);
}

void DataToFirebase(float temp_value, float humi_value, float Moisture_value ){
      Firebase.setFloat("Floor1/Temperature",temp_value);
      Firebase.setFloat("Floor1/Humidity",humi_value);
      Firebase.setFloat("Floor1/Moisture1",Moisture_value);
//    Firebase.setFloat("Floor1/Moisture2",Moisture2_value);
//    Firebase.setFloat("Floor1/Moisture3",Moisture3_value);
//    Firebase.setFloat("Floor1/Moisture4",Moisture4_value);
}
