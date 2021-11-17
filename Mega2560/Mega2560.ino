//토양 습도 센서 x4
#define MOISTURE_1 A0
#define MOISTURE_2 A1
#define MOISTURE_3 A2
#define MOISTURE_4 A3

int trigger_pin = 2;
int echo_pin = 3;

int time;
int distance; 
String values;

 void setup() { 
  
 pinMode (trigger_pin, OUTPUT); 
 
 pinMode (echo_pin, INPUT);
 
 //Initializes the serial connection at 9600 to sent sensor data to ESP8266.
 Serial.begin(9600); 
 delay(2000);  
  
 }
 
void loop() {
  analogWrite(3, 225);
  // get sensors data and put in to values variables as a string.
   values = (get_Moisture1_value()+','+get_Moisture2_value()+','+get_Moisture3_value()+','+get_Moisture4_value()+',');
       delay(1000);
       // removed any buffered previous serial data.
       Serial.flush();
       delay(1000);
       // sent sensors data to serial (sent sensors data to ESP8266)
       Serial.print(values);
//       Serial.print(get_Moisture1_value());
//       Serial.print("/");
//       Serial.print(get_Moisture2_value());
//       Serial.print("/");
//       Serial.print(get_Moisture3_value());
//       Serial.print("/");
//       Serial.println(get_Moisture4_value());
//       delay(1000);
}

String get_Moisture1_value(){ 
    float res;
    float M = analogRead(MOISTURE_1);   //토양 습도를 측정합니다.

    int i = 1023;
    int j = 100;
    res = M*j/i;                //%로 변환
    return String(res);  
}

String get_Moisture2_value(){ 
    float res;
    float M = analogRead(MOISTURE_2);   //토양 습도를 측정합니다.

    int i = 1023;
    int j = 100;
    res = M*j/i;                //%로 변환
    return String(res);  
}

String get_Moisture3_value(){ 
    float res;
    float M = analogRead(MOISTURE_3);   //토양 습도를 측정합니다.

    int i = 1023;
    int j = 100;
    res = M*j/i;                //%로 변환
    return String(res);  
}

String get_Moisture4_value(){ 
    float res;
    float M = analogRead(MOISTURE_4);   //토양 습도를 측정합니다.

    int i = 1023;
    int j = 100;
    res = M*j/i;                //%로 변환
    return String(res);  
}
 
