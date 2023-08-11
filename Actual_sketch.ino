#include <RTClib.h>
#include <Wire.h>
RTC_DS3231 rtc;
char t[32];
String a="Thread guage";
String b="11:07:2023";
String c="11:08:2023 ";
void setup()
{
  
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  rtc.adjust(DateTime(2023, 8, 11, 12, 52, 0));
}
void loop()
{
  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d:%02d ", now.day(), now.month(), now.year());  
 // Serial.print(F("Date/Time: "));
 // Serial.println(t);
  delay(1000);
  if(c==t)
  {
    Serial.println("Guage:"+ a);
    
    Serial.println("due expired please deliver it");
  
  }
}