//  #include <Blynk.h>
 #include <ESP8266WiFi.h>  
 #include <ESP8266HTTPClient.h>  
 #include <WiFiClient.h>
//  #include <LiquidCrystal.h>
//  #include <BlynkSimpleEsp8266.h>

 String thingSpeakAddress= "http://api.thingspeak.com/update?";  
 String writeAPIKey;  
 String tsfield1Name;  
 String request_string,request_string1,request_string2;  
 HTTPClient http;  

#define BLYNK_TEMPLATE_ID "TMPL3EL5Iwmaj"
#define BLYNK_TEMPLATE_NAME "Smart Agriculture Monitoring System IoT"
#define BLYNK_AUTH_TOKEN "4PcMWZUmrXmRfEnGuE0HpJhRAaHzpPAF"
#define BLYNK_PRINT Serial
BlynkTimer timer;


 String apiKey = "AR7MR45XVYCB7QA4";   // Enter your Write API key from ThingSpeak  
 const char *ssid = "WifiConnection1";     // replace with your wifi ssid and wpa2 key  
 const char *pass = "12345678";    // WIFI Password    
 const char* server = "api.thingspeak.com";  
 WiFiClient client;  
int pirpin = 13; //D7
int pirstate = LOW;
int relay_pin = 5; //D1
// int push_btn = 10;
#define temp_pin D0
// void checkPhysicalButton();
int relay1State = LOW;
// int pushButton1State = HIGH;

 float celsius = 0.0;
//  String answer = "";
// void checkPhysicalButton()  
//  {  
//   if (digitalRead(push_btn) == LOW) {  
//    // pump_status is used to avoid sequential toggles  
//    if (pump_status != LOW) {  
//     // Toggle Relay state  
//     pump_status = !pump_status;  
//     digitalWrite(relay_pin, pump_status); 
//     Serial.println("Button Pushed");
//    }  
//    pump_status = LOW;  
//   } else {  
//    pump_status = HIGH;  
//   }  
//  }  


void setup()  
 {  
  Serial.begin(9600);  
  Serial.println("START"); 
  // pinMode(12 , OUTPUT); 
  pinMode(A0 , INPUT);
  pinMode(13 , INPUT); 
  pinmode(relay_pin , INPUT);
  Serial.print("calibrating sensor ");
  for(int i = 0; i < 20; i++){
      Serial.print(".");
      delay(1000);
      }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
  WiFi.begin("WifiConnection1","12345678");  
  while ((!(WiFi.status() == WL_CONNECTED))){  
   delay(300);  
   Serial.println("Connecting...");  
  }  
  Serial.println("I AM CONNECTED"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
}  
void loop()  
 {  
   digitalWrite(pirpin , LOW);
  Serial.println("LOOP Starting...");
   if (client.connect("api.thingspeak.com",80))  
   {  
      
    request_string += "https://api.thingspeak.com/update?api_key=AR7MR45XVYCB7QA4&field2=0";
    http.begin(client , request_string);  
    http.GET();  
    http.end();  
   }  
   if (client.connect("api.thingspeak.com",80))  
   {  
     Serial.println("Reading Soil sensor data..");
     if(analogRead(A0) <= 200){
       digitalWrite(relay_pin , HIGH);
     }
     else{
       digitalWrite(relay_pin , LOW);
     }
    request_string += "https://api.thingspeak.com/update?api_key=AR7MR45XVYCB7QA4&field1=";;  
    request_string += analogRead(A0);
    http.begin(client , request_string);  
    http.GET();  
    http.end();  
   }  
   delay(10);
   if (client.connect("api.thingspeak.com",80))  
   {  
     Serial.println("Reading Motion Sensor data");
     delay(3000);
     Serial.println(digitalRead(pirpin));
     delay(3000);
    request_string2 += "https://api.thingspeak.com/update?api_key=AR7MR45XVYCB7QA4&field2=";
    if(digitalRead(pirpin) == HIGH){
    Serial.println("Motion Detected.");}
    request_string2 += digitalRead(pirpin);
    http.begin(client , request_string2);  
    http.GET();  
    http.end();  
   }  
   delay(10); 
   if (client.connect("api.thingspeak.com",80))  
   {  
    int analogValue = digitalRead(temp_pin);
    float millivolts = (analogValue/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
    celsius = millivolts*10;
    Serial.print("in ℃= ");
    Serial.println(celsius);
    float fahrenheit = ((celsius * 9)/5 + 32);
    Serial.print("Farenheit= ");
    Serial.print(fahrenheit);
    delay(100);
    Serial.print("\nTemperature: ");
    Serial.print(celsius); 
    Serial.print("℃, Send to Thingspeak.");
    delay(100);
    request_string1 = "https://api.thingspeak.com/update?api_key=AR7MR45XVYCB7QA4&field4=";
    celsius = int(celsius);
    request_string1 += celsius;
    http.begin(client , request_string1);  
    http.GET();  
    http.end();  
   }  
   delay(10);
              if (client.connect("184.106.153.149",80))  //  "184.106.153.149" or api.thingspeak.com  
            { 
                client.print("POST /update HTTP/1.1\n");  
                client.print("Host: api.thingspeak.com\n");  
                client.print("Connection: close\n");  
                client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");  
                client.print("Content-Type: application/x-www-form-urlencoded\n");  
                client.print("Content-Length: ");  
                client.print("\n\n");   
                Serial.print(" Soil Sensor ");  
                Serial.print(analogRead(A0));  
                Serial.println(", Send to Thingspeak.");
             }  
             if (client.connect("api.thingspeak.com",80))  
   {  
    request_string2 = thingSpeakAddress;  
    request_string2 += "key=";  
    request_string2 += "AR7MR45XVYCB7QA4";  
    request_string2 += "&";  
    request_string2 += "field2";  
    request_string2 += "=0";
    http.begin(client , request_string2);  
    http.GET();  
    http.end();  
   }
      client.stop();  

      Serial.println("Waiting...");  
  // thingspeak needs minimum 15 sec delay between updates  
  delay(10);  
}