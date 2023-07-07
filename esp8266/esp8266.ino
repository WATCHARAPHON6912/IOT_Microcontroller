#include "EEPROM.h"
#include <FILM69.h>
#define BLYNK_PRINT Serial
FILM69 F;
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "DHT.h"
#define DHTPIN D6

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

int period1 = 1000; //ระยะเวลาที่ต้องการรอ
int last_time1 = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวนloop

int SW1, SW2, SW3;
int TIME1, TIME2, TIME3;
int time1, time2, time3;
int temp_set;
BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

void setup() {
  Serial.begin(115200);
  delay(3000);
  EEPROM.begin(512);
  dht.begin();

  //setwifi();
  if (F.DR(D0) == 1) {
    setwifi();
    Serial.println("AP MODE");

  } else {
    Connect();
    Serial.println("CL MODE");
  }
}
void loop() {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  Blynk.run();
  timer.run();
  Blynk.virtualWrite(V7, t);
  Blynk.virtualWrite(V8, h);

  if (t >= temp_set) {
    F.DW(D5, 1);
  } else {
    F.DW(D5, 0);
  }

  if (SW1 == 1) {
    time1 = 0;
  }
  if (SW2 == 1) {
    time2 = 0;
  }
  if (SW3 == 1) {
    time3 = 0;
  }
  if (time1 <= TIME1) {
    F.DW(D1, 1);
  } else {
    F.DW(D1, 0);
  }
  if (time2 <= TIME2) {
    F.DW(D2, 1);
  } else {
    F.DW(D2, 0);
  }
  if (time3 <= TIME3) {
    F.DW(D3, 1);
  } else {
    F.DW(D3, 0);
  }
  Serial.print(time1);
  Serial.print(" | ");
  Serial.print(TIME1);
  Serial.print("   #   ");
  Serial.print(time2);
  Serial.print(" | ");
  Serial.print(TIME2);
  Serial.print("   #   ");
  Serial.print(time3);
  Serial.print(" | ");
  Serial.print(TIME3);
  Serial.println("   #   ");

  if ( millis() - last_time1 > period1) {
    last_time1 = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
    time1++;
    time2++;
    time3++;
    if (time1 >= 30000) {
      time1 = 30000;
    }
    if (time2 >= 30000) {
      time2 = 30000;
    }
    if (time3 >= 30000) {
      time3 = 30000;
    }

  }


}

BLYNK_WRITE(V0) {
  SW1 = param.asInt();
}
BLYNK_WRITE(V1) {
  SW2 = param.asInt();
}
BLYNK_WRITE(V2) {
  SW3 = param.asInt();
}
BLYNK_WRITE(V3) {
  TIME1 = param.asInt();
}
BLYNK_WRITE(V4) {
  TIME2 = param.asInt();
}
BLYNK_WRITE(V5) {
  TIME3 = param.asInt();
}
BLYNK_WRITE(V6) {
  temp_set = param.asInt();
}
