
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "OPPOReno2F"
#define WIFI_PASSWORD "bro45678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyC218dH6k9CjH1wXcZKlrJoSEv5HewDf84"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://gauges-management-system-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
int Balarm = D8;
#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 
RTC_DS3231 rtc;
char t[32];

void setup(){
  pinMode(Balarm, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  lcd.init();         
  lcd.backlight();
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
 // rtc.adjust(DateTime(2023, 8, 11, 12, 52, 0));

}

void loop(){

  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d:%02d ", now.day(), now.month(), now.year());  
  Serial.println(t);
  String date1;
  String gn;
  String loc;
  if (Firebase.RTDB.getString(&fbdo, "/Gauge/F324001D/ed/"))
  {
    if(fbdo.dataType()="string")
    {
           date1= fbdo.stringData();
    }
  }
  if (Firebase.RTDB.getString(&fbdo, "/Gauge/F324001D/l/"))
  {
    if(fbdo.dataType()="string")
    {
           loc= fbdo.stringData();
    }
  }
  if (Firebase.RTDB.getString(&fbdo, "/Gauge/F324001D/gN/"))
  {
    if(fbdo.dataType()="string")
    {
           gn= fbdo.stringData();
    }
  }
  Serial.println(date1);
 // Serial.println(gn);
   if (date1==t)
   {
     digitalWrite(Balarm, HIGH);
     delay(1000);
     digitalWrite(Balarm, LOW);
     lcd.setCursor(2,0);
     lcd.print("F324001D");
     lcd.setCursor(2,1);
     lcd.print(gn);
     delay(1000);
     lcd.clear();
     lcd.setCursor(2,0);
     lcd.print("due expired");
     lcd.setCursor(2,1);
     lcd.print(loc);
     
    

   }
}