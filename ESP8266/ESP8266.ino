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

//String get_temp_value(){
//    String temp = Firebase.getString("Log/Temperature");
//    delay(50);
//    return String(temp);
//}
//
//String get_humi_value(){ 
//   
//    String humi = Firebase.getString("Log/Humidity");
//    delay(50);
//    return String(humi);  
//}
//
//String get_Light_value(){ 
//    String light = Firebase.getString("Log/Light");
//    delay(50);
//    return String(light);
//}
