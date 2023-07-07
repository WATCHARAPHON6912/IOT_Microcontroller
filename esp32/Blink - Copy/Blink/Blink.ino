
#define BLYNK_AUTH_TOKEN "9mYmZSAI0gaUF07m-n-tx7HOuxFev6il"
#include "DHT.h"
#define DHTPIN 23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;
unsigned long period = 1000; //ระยะเวลาที่ต้องการรอ
unsigned long last_time = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวนloop
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "FILM69";
char pass[] = "00000000";
int Relay1, Relay2, Relay3, Relay4, tempset, analogset;
int analogin, Mode, humset, i, j, TIME_CHECK, TIME_RUN, TIME;
int analogpin = 15;
bool Status;
float HUM, TEMP;
int pin = 21;
void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin();
  pinMode(analogpin, INPUT);
  pinMode(pin, INPUT);
  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "oasiskit.com", 8080);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  timer.run();
  //Serial.println(digitalRead(pin));
  if ( millis() - last_time > period) {
    last_time = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
    TIME++;
  }
  if (Mode == 1) {
    if (j == 0) {
      if (i >= 1) {
        i = -1;
      }
      i++;
    }
    j++;
  }
  else {
    j = 0;
  }
  Serial.println(WifiSignal());
  Blynk.virtualWrite(V0, WifiSignal());

}
void RUN() {


}
void BLYNK_V_WRITE(int v, int io) {
  Blynk.virtualWrite(v, io);
}
void IO(int pin , int DIGI) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, DIGI);
}

BLYNK_WRITE(V1) {
  Relay1 = param.asInt();
}
int WifiSignal() {
  //หาค่าสัญญาณ RSSI ของ WIFI แล้ว MAP ให้อยู่ในรูปของเปอร์เซ็นต์
  int CurrentWiFiSignal = WiFi.RSSI();
  
  if(CurrentWiFiSignal > -40) CurrentWiFiSignal = -40;
  if(CurrentWiFiSignal < -90) CurrentWiFiSignal = -90;
  
  int WifiSignal = map(CurrentWiFiSignal, -90, -40, 0, 100);
  
  return WifiSignal;
}
