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
      u8g2.print("Temp : " + temp_value+ " °C");
      u8g2.setCursor(0, 35);
      u8g2.print("Humi : " + humi_value + " %"); 
      u8g2.setCursor(0, 55);
      u8g2.print("Light : " + Light_value + " %"); 
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
 
  
