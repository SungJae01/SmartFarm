#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "test1-69dfc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "QOLdKEPtKAp8DcK1YBHIUo1FOobHD7EX223faEfB"
//우리집
//#define WIFI_SSID "U+Net3CA8"
//#define WIFI_PASSWORD "30AB@PFB97"
//Camp51.9
#define WIFI_SSID "KT_GiGA_ABA8"
#define WIFI_PASSWORD "7cb72ck295"

String values,sensor_data;

void setup() {
  //Initializes the serial connection at 9600 get sensor data from arduino.
   Serial.begin(9600);
   
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
}
void loop() {

 bool Sr =false;
 
  while(Serial.available()){
    
        //get sensor data from serial put in sensor_data
        sensor_data=Serial.readString(); 
        Sr=true;    
        
    }
  
delay(100);

  if(Sr==true){  
    
    values=sensor_data;
    
    int fristCommaIndex = values.indexOf(',');
    int secondCommaIndex = values.indexOf(',', fristCommaIndex+1);
    int thirdCommaIndex = values.indexOf(',', secondCommaIndex + 1);
    int forthCommaIndex = values.indexOf(',', thirdCommaIndex + 1);
    
    
    String Moisture1_value = values.substring(0, fristCommaIndex);
    String Moisture2_value = values.substring(fristCommaIndex+1, secondCommaIndex);
    String Moisture3_value = values.substring(secondCommaIndex+1, thirdCommaIndex);
    String Moisture4_value = values.substring(thirdCommaIndex+1, forthCommaIndex);
  
     
    Firebase.setString("Floor1/Moisture1",Moisture1_value);
    Firebase.setString("Floor1/Moisture2",Moisture2_value);
    Firebase.setString("Floor1/Moisture3",Moisture3_value);
    Firebase.setString("Floor1/Moisture4",Moisture4_value);
    
    if (Firebase.failed()) {  
        return;
    }
  }   
}
