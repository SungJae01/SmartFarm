#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <iostream>
// 파이어베이스 주소, 사용자 비밀번호
#define FIREBASE_HOST "FIREBASE_HOST"
#define FIREBASE_AUTH "FIREBASE_AUTH"
// 와이파이 이름, 비밀번호
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"


String values,sensor_data;
int n = 0;
int i = 0;
char *str[] = {"연결","연결.","연결..","연결..."};

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

  Firebase.setString("Connect","연결중...");
  delay(3000);
  
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
    
    // values에서 쉼표 인덱스 가져오기
    int fristCommaIndex = values.indexOf(',');
    int secondCommaIndex = values.indexOf(',', fristCommaIndex+1);
    int thirdCommaIndex = values.indexOf(',', secondCommaIndex+1);
    int forthCommaIndex = values.indexOf(',', thirdCommaIndex+1);
    int fifthCommaIndex = values.indexOf(',', forthCommaIndex+1);
    
    // 쉼표로 분할하여 values에서 센서 데이터를 가져오고 각 변수에 넣기 
    String temp_value = values.substring(0, fristCommaIndex);
    String humi_value = values.substring(fristCommaIndex+1, secondCommaIndex);
    String Light_value = values.substring(secondCommaIndex+1, thirdCommaIndex);
    String Water_value = values.substring(thirdCommaIndex+1, forthCommaIndex);
    String Moisture_value = values.substring(forthCommaIndex+1, fifthCommaIndex);
    
    // 데이터 베이스에 값 저장
    Firebase.setString("Log/Temperature",temp_value);
    Firebase.setString("Log/Humidity",humi_value);
    Firebase.setString("Log/Light",Light_value);
    Firebase.setString("Log/Water",Water_value);
    Firebase.setString("Log/Moisture",Moisture_value);
    Firebase.setFloat("Log/number", n++);

    int temp_value_num = temp_value.toInt();          //문자열 값 정수로 변환
    int Moisture_value_num = Moisture_value.toInt();

    if(temp_value_num >= 27){
      Firebase.setString("Log/Moter", "모터 작동중...");
    }
    else if(temp_value_num < 27){
      Firebase.setString("Log/Moter", "모터 작동 중지");
    }
    if(temp_value_num >= 40){
      Firebase.setString("Log/WaterPump", "워터 펌프 작동중...");
    }
    else if(temp_value_num < 40){
      Firebase.setString("Log/WaterPump", "워터 펌프 작동 중지");
    }
    
    if(n == 101){
      n = 0;
    }
    
    Firebase.setString("Connect",str[i++]);
    if(i == 4){
      i = 0;
    }


    String number = Firebase.getString("Log/number");
    Serial.print(number);

    
    delay(300);
    
    if (Firebase.failed()) {
        return;
    }
  
  }   
}
