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

#define ENA 5
#define EN1 4
#define EN2 3
#define ENB 8
#define EN3 7
#define EN4 6

DHT dht(DHTPIN, DHTTYPE);
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

String values;
int L;
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
int Motor_speed_A=255;
int Motor_speed_B=255;
void setup(){
 //Initializes the serial connection at 9600 to sent sensor data to ESP8266.
  Serial.begin(9600); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // PWM 제어핀 출력 설정
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  // 방향 제어핀 출력 설정
  pinMode(EN1,OUTPUT);
  pinMode(EN2,OUTPUT);
  pinMode(EN3,OUTPUT);
  pinMode(EN4,OUTPUT);
  
  dht.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  
}

 
void loop() {

      if(dht.readTemperature() >= 27)    //온도에 따른 팬 작동
      {
        digitalWrite(EN3, LOW);   // 모터B 설정 
        digitalWrite(EN4, HIGH);
        analogWrite(ENB, Motor_speed_B);
      }
      else if(dht.readTemperature() < 27)
      {
        digitalWrite(EN3, LOW);   // 모터B 설정 
        digitalWrite(EN4, LOW);
      }
      float Moister_value_num = get_Moisture_value().toInt();
      if(Moister_value_num >= 40)
      {
        digitalWrite(EN1, LOW);   // 모터A 설정 
        digitalWrite(EN2, HIGH);
        analogWrite(ENA, Motor_speed_A);
      }
      else if(Moister_value_num < 40)
      {
        digitalWrite(EN1, LOW);   // 모터A 설정 
        digitalWrite(EN2, LOW);
      }

    //get comma indexes from values variable
    int fristCommaIndex = values.indexOf(',');
    int secondCommaIndex = values.indexOf(',', fristCommaIndex+1);
    int thirdCommaIndex = values.indexOf(',', secondCommaIndex+1);
    int forthCommaIndex = values.indexOf(',', thirdCommaIndex+1);
    int fifthCommaIndex = values.indexOf(',', forthCommaIndex+1);
    
    //get sensors data from values variable by  spliting by commas and put in to variables  
    String temp_value = values.substring(0, fristCommaIndex);
    String humi_value = values.substring(fristCommaIndex+1, secondCommaIndex);
    String Light_value = values.substring(secondCommaIndex+1, thirdCommaIndex);
    String Water_value = values.substring(thirdCommaIndex+1, forthCommaIndex);
    String Moisture_value = values.substring(forthCommaIndex+1, fifthCommaIndex);

      // get sensors data and put in to values variables as a string.
    values = (get_temp_value()+','+ get_humi_value() +','+ get_Light_value() +',' + get_Water_value() + ',' + get_Moisture_value() + ',');
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
//      u8g2.setCursor(0, 0);
//      u8g2.print("Temp : " + temp_value+ " °C");
//      u8g2.setCursor(0, 15);
//      u8g2.print("Humi : " + humi_value + " %"); 
//      u8g2.setCursor(0, 30);
//      u8g2.print("Light : " + Light_value + " %"); 
      u8g2.setCursor(0, 25);
      u8g2.print("Water : " + Water_value + " cm"); 
      u8g2.setCursor(0, 45);
      u8g2.print("Moisture:" + Moisture_value + "%"); 
    } while ( u8g2.nextPage());
}

String get_temp_value(){  
    float t = dht.readTemperature();    // 온도를 측정합니다.
    return String(t);  
}

String get_humi_value(){ 
   
    float h = dht.readHumidity();   // 습도를 측정합니다.
    return String(h);  
      
}

String get_Light_value(){ 
    float res;
    float L = analogRead(A0);   // 조도를 측정합니다.
    int i = 1023;
    int j = 100;
    res = L*j/i;
    return String(res);  
    
}

String get_Water_value(){ 
    long duration, cm; 
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    pinMode(echoPin, INPUT);    //수위를 측정합니다.
    duration = pulseIn(echoPin, HIGH);

    cm = (duration/2) / 29.1;   //cm로 변환
    return String(cm);  
}

String get_Moisture_value(){
    float res;
    float M = analogRead(A1);

    int i = 1023;
    int j = 100;
    res = M*j/i;
    return String(res);  
      
}
