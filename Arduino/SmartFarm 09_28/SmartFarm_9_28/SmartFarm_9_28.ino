#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Arduino.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <iostream>
#include <string>  


// Set these to run example.
//#define FIREBASE_HOST "kimlab-21be3-default-rtdb.firebaseio.com"
//#define FIREBASE_AUTH "r5hljBYYrBhfysIZuiKEuQaulKtaDSYJumICdYgb"

#define FIREBASE_HOST "test1-69dfc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "QOLdKEPtKAp8DcK1YBHIUo1FOobHD7EX223faEfB"
//우리집
#define WIFI_SSID "U+Net3CA8"
#define WIFI_PASSWORD "30AB@PFB97"
//Camp51.9
//#define WIFI_SSID "KT_GiGA_ABA8"
//#define WIFI_PASSWORD "7cb72ck295"



#define DHTPIN 0
#define DHTTYPE DHT22
#define Relay 12
//#define Switch 14
#define MOISTURE1 A0

DHT dht(DHTPIN, DHTTYPE);

int timeSinceLastRead = 0;
int StateData[3];     // 1층 식물 조도, 최고온도, 최저온도
int ON_OFF;
String select;
//float now_temp=0 , temp;
////모터 속도 조절
//int Motor_speed_A=255;
//int Motor_speed_B=255;


void setup() {
  Serial.begin(9600);
  dht.begin();

//  // PWM 제어핀 출력 설정
//  pinMode(ENA,OUTPUT);
//  pinMode(ENB,OUTPUT);
//  // 방향 제어핀 출력 설정
//  pinMode(EN1,OUTPUT);
//  pinMode(EN2,OUTPUT);
//  pinMode(EN3,OUTPUT);
//  pinMode(EN4,OUTPUT);

//  pinMode(Relay, OUTPUT);
//  pinMode(Switch,INPUT_PULLUP);

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

//  // initialize with the I2C addr 0x3C
//  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
//
//  // Clear the buffer.
//  display.clearDisplay();
}

int k = 1;
//float temp_value=0, temp;
//temp=temp_value;
void loop() {
  
    ON_OFF = Firebase.getFloat("Floor1/start");
    Serial.println(ON_OFF);

    if(ON_OFF == 1){   // 모니터링 작동 시작
      select = Firebase.getString("Floor1/select");
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

    Serial.println(select);
    
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
    int Moisture_value = analogRead(MOISTURE1);
    
    Serial.print("온도 : ");
    Serial.println(Firebase.getFloat("Floor1/Temperature"));
    Serial.print("습도 : ");
    Serial.println(Firebase.getFloat("Floor1/Humidity"));
    Serial.print("토양 습도 : ");
    Serial.println(Firebase.getFloat("Floor1/Moisture"));
    Serial.println("-------------------------------------");
  
    Firebase.setFloat("Floor1/Temperature",temp_value);
    Firebase.setFloat("Floor1/Humidity",humi_value);
    Firebase.setFloat("Floor1/Moisture",Moisture_value);

    
    
    String tempString = "";
    String humiString = "";
    tempString.concat(temp_value);
    humiString.concat(humi_value);

//    Water_Pump(Moisture_value);

  delay(1000);
}

float get_Moisture_value(){
    float res;
    float M = analogRead(MOISTURE1);   //토양 습도를 측정합니다.

    int i = 1023;
    int j = 100;
    res = M*j/i;                //%로 변환
    return res;  
}

//void Water_Pump(float Moisture_value1,float Moisture_value2,float Moisture_value3,float Moisture_value4){
//  
//  if(Moisture_value1 < 80){
//      digitalWrite(EN1, LOW);   // 워터 펌프 작동
//      digitalWrite(EN2, HIGH);
//      analogWrite(ENA, Motor_speed_A);
//      delay(5000);
//      digitalWrite(EN1, LOW);   // 워터 펌프 중지
//      digitalWrite(EN2, LOW);
//      delay(2000);
//   }
//   else{
//     digitalWrite(EN1, LOW);   // 워터 펌프 중지
//     digitalWrite(EN2, LOW);
//   }
//   
//}
//
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
