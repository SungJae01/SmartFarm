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
#define DHTPIN 2 //D4
#define DHTTYPE DHT22

//릴레이 x ?
#define Relay 0      //d6

//시프트레지스터 74HC595
#define DATA_PIN 14   //D5
#define LATCH_PIN 12  //D6
#define CLOCK_PIN 13  //D7

//LED 바 x 2
#define LED1 1    //rx
#define LED2 3    //tx

DHT dht(DHTPIN, DHTTYPE);

int timeSinceLastRead = 0;
int StateData[3];     // 1층 식물 조도, 최고온도, 최저온도
String select;
//float now_temp=0 , temp;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  
  digitalWrite(DATA_PIN,LOW);
  digitalWrite(LATCH_PIN,LOW);
  digitalWrite(CLOCK_PIN,LOW);

  //LED바
  pixels.setBrightness(BRIGHTNESS);    //  BRIGHTNESS 만큼 밝기 설정 
  pixels.begin();                      //  Neopixel 제어를 시작
  pixels.show();                       //  Neopixel 동작 초기화 합니다
  
  //OLED
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();

  pinMode(Relay, OUTPUT);
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

  // initialize with the I2C addr 0x3C
  // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  // Clear the buffer.
  //display.clearDisplay();
}
bool oldState = HIGH;
int showType = 0;
int k = 1;
char input = 0;
int ON_OFF;
float temp_value;
float humi_value;

void loop() {
    for(int i = 1; i < 255; i++){
      analogWrite(15, i);
      delay(5);
    }
    for(int j = 255; j > 0; j--){
      analogWrite(15, j);
      delay(5);
    }
  
    ON_OFF = Firebase.getInt("Floor1/start");
    Serial.println(ON_OFF);
    DataSetup(ON_OFF);
    OLED(ON_OFF);
    
//    for(int i = 0; i<3; i++){
//      Serial.print(i);
//      Serial.print(" : ");
//      Serial.println(StateData[i]);
//      delay(100);
//    }

    temp_value = dht.readTemperature();
    humi_value = dht.readHumidity();
    
    Serial.print("온도 : ");
    Serial.print(temp_value);
    Serial.println("°C");
    Serial.print("습도 : ");
    Serial.print(humi_value);
    Serial.println("%");
    Serial.println("-------------------------------------┐");

    if(humi_value > 40){
      digitalWrite(Relay, HIGH);
      Firebase.setBool("Floor1/Steam",false);
    }
    else{
      digitalWrite(Relay, LOW);
      Firebase.setBool("Floor1/Steam",true);
    }
    

    DataToFirebase(temp_value, humi_value);

//  Water_Pump(Moisture1_value,Moisture2_value,Moisture3_value,Moisture4_value);

//  pixels.setPixelColor(NUMPIXELS, pixels.Color(255,0,0));
//  pixels.show();
  WaterPump();
      delay(1000);
}

float get_temp_value(){
    float temp_value = dht.readTemperature();
    return temp_value;  
}

void WaterPump(){
  if(Firebase.getFloat("Floor1/Moisture1") < 20){
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b10000000);
    Firebase.setBool("Floor1/WaterPump",true);
    digitalWrite(LATCH_PIN, HIGH);
    delay(1);
    digitalWrite(LATCH_PIN, LOW);
  } else{
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b00000000);
    Firebase.setBool("Floor1/WaterPump",false);
    digitalWrite(LATCH_PIN, HIGH);
    delay(1);
    digitalWrite(LATCH_PIN, LOW);
  }
  
}

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

void OLED(int ON_OFF){
  u8g2.clearBuffer();
  if(ON_OFF == 2){
    while(1){
      u8g2.clearBuffer();
      delay(500);
      u8g2.setFont(u8g2_font_unifont_t_korean1);  
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(9, 37);
        u8g2.print("식물 심는중");
      } while ( u8g2.nextPage() );
      delay(300);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_unifont_t_korean1);  
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(9, 37);
        u8g2.print("식물 심는중.");    // Korean "Hello World"
      } while ( u8g2.nextPage() );
      delay(300);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_unifont_t_korean1);  
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(9, 37);
        u8g2.print("식물 심는중..");    // Korean "Hello World"
      } while ( u8g2.nextPage() );
      delay(300);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_unifont_t_korean1);  
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(9, 37);
        u8g2.print("식물 심는중...");    // Korean "Hello World"
      } while ( u8g2.nextPage() );
      if(Firebase.getInt("Floor1/start") != 2){
        return;
      }
    }
   }
    if(ON_OFF == 1) {
      u8g2.clearBuffer(); 
      u8g2.firstPage();
      do {
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_streamline_all_t); 
        u8g2.drawGlyph(54,46, 0x00db);
        u8g2.setFont(u8g2_font_streamline_all_t); 
        u8g2.drawGlyph(54,37, 0x00d7);
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
        u8g2.drawGlyph(119, 9, 0x00f7);
        
        if(1){
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(1, 9, 0x0103);
        }
        else{
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(1, 9, 0x011f);
        }
        
        if(1){
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(10, 9, 0x0048);
        }
        else if(0){
          u8g2.setFontDirection(0);
          u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
          u8g2.drawGlyph(10, 9, 0x0053);
        }

        if(1){
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
    if(ON_OFF == 0) {
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
    delay(100);
}

void DataSetup(int ON_OFF){

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
    delay(100);
}

void DataToFirebase(float temp_value, float humi_value){
      Firebase.setFloat("Floor1/Temperature",temp_value);
      if (Firebase.failed()) {
        Serial.print("Temperature failed:");
        Firebase.setString("Temperature","Error");
        Serial.println(Firebase.error());  
      }
      delay(100);
      Firebase.setFloat("Floor1/Humidity",humi_value);
      if (Firebase.failed()) {
        Serial.print("Humidity failed:");
        Firebase.setString("Humidity","Error");
        Serial.println(Firebase.error());
        
      }
      delay(100);
}
