#define BLYNK_AUTH_TOKEN            "2fli26MIRgLmGxhmGyU2sdys8k1YfgNI"
#include "DHT.h"
#include "EEPROM.h"
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
//char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "FILM69";
//char pass[] = "00000000";
int  tempset, analogset;
bool Relay1, Relay2, Relay3, Relay4;
int analogin, Mode, humset, i, j, TIME_CHECK, TIME_RUN, TIME;
int analogpin = 34;
bool Status;
int jj, jjj;
float HUM, TEMP;
int pin = 21;
int RelayStatus = 0;
void setup() {
 
  Serial.begin(115200);
  dht.begin();
  if (!EEPROM.begin(1000)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    ESP.restart();
  }

  pinMode(analogpin, INPUT);
  pinMode(pin, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  /*
  Serial.println("START");
  delay(1000);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  */
  delay(1000);
  Serial.println("OK");

  if (digitalRead(pin) == 0) {
    setwifi();
    Serial.println("AP MODE");
  } else {
    Serial.println("CL MODE");
    Connect();
  }
  Relay1=1;
  Relay2=1;
  Relay3=1;
  Relay4=1;
  //ESP.restart();
}

void loop()
{
  
  Blynk.run();
  timer.run();
  if(digitalRead(pin) == 0 &&digitalRead(18) == 0){
    ESP.restart();
  }
  //Serial.println(digitalRead(pin));
  if ( millis() - last_time > period) {
    last_time = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
    TIME++;
  }
  if (Mode == 1 || digitalRead(pin) == 0) {
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



  RUN();

  analogin = analogRead(analogpin);
  analogin = map(analogin, 0, 4095, 100, 0);
  HUM = dht.readHumidity();
  TEMP = dht.readTemperature();


  /*
    Serial.print("  Relay1 : ");
    Serial.print(Relay1);
    Serial.print("  Relay2 : ");
    Serial.print(Relay2);
    Serial.print("  Relay3 : ");
    Serial.print(Relay3);
    Serial.print("  Relay4 : ");
    Serial.print(Relay4);
    Serial.print(" SET_TEMP : ");
    Serial.print(tempset);
    Serial.print(" SET_ANALOG : ");
    Serial.print(analogset);
    Serial.print(" TEMP : ");
    Serial.print(TEMP);
    Serial.print(" *C ");
    Serial.print(" HUM : ");
    Serial.print(HUM);
    Serial.print(" % ");
    Serial.print(" ANALOG_IN : ");
    Serial.print(analogin);
    Serial.print(" SET_HUM : ");
    Serial.print(humset);
    Serial.print(" MODE : ");
    Serial.print(Mode);
    Serial.print(" i : ");
    Serial.print(i);
    Serial.print(" TIME_CHECK : ");
    Serial.print(TIME_CHECK);
    Serial.print(" TIME_RUN : ");
    Serial.print(TIME_RUN);
    Serial.print(" Status : ");
    Serial.print(Status);
    Serial.print(" TIME_CHECK : ");
    Serial.print(TIME_CHECK);
    Serial.print(" TIME_RUN : ");
    Serial.print(TIME_RUN);
    Serial.print(" TIME : ");
    Serial.println(TIME);
  */

  Serial.println(analogin);

  Blynk.virtualWrite(V7, analogin);
  Blynk.virtualWrite(V8, TEMP);
  Blynk.virtualWrite(V9, HUM);
  Blynk.virtualWrite(V10, i);
  Blynk.virtualWrite(V20, WifiSignal());
}
void RUN() {
  if (TIME >= TIME_CHECK + TIME_RUN) {
    TIME = 0;
  }
  //#################### MODE MUNAUL ##############################################
  if (i == 0) {
    if (RelayStatus >= 3)RelayStatus = 3; RelayStatus++;
    if (RelayStatus == 1) {
      BLYNK_V_WRITE(V1, Relay1);
      BLYNK_V_WRITE(V2, Relay2);
      BLYNK_V_WRITE(V3, Relay3);
      BLYNK_V_WRITE(V4, Relay4);
    }
    if (digitalRead(18) == 0) {
      if (jj == 0) {
        Relay1 = !Relay1;
        BLYNK_V_WRITE(V1, Relay1);
      }
      jj++;
    }
    else {
      jj = 0;
    }


    if (digitalRead(19) == 0) {
      if (jjj == 0) {
        Relay2 = !Relay2;
        BLYNK_V_WRITE(V2, Relay2);
      }
      jjj++;
    }
    else {
      jjj = 0;
    }
    IO(26, Relay1);
    IO(25, Relay2);
    IO(33, Relay3);
    IO(32, Relay4);
  }
  //#################### MODE AUTO ################################################
  else {
    RelayStatus = 0;

    if (TIME_CHECK == 0 || TIME > TIME_CHECK && TIME < TIME_CHECK + TIME_RUN) {
      if (HUM >= humset) {
        IO(26, 1);
        BLYNK_V_WRITE(V1, 1);
      } else {
        IO(26, 0);
        BLYNK_V_WRITE(V1, 0);
      }

      if (analogin >= analogset) {
        IO(25, 1);
        IO(33, 1);
        IO(32, 1);
        BLYNK_V_WRITE(V2, 1);
        BLYNK_V_WRITE(V3, 1);
        BLYNK_V_WRITE(V4, 1);
      } else {
        IO(25, 0);
        IO(33, 0);
        IO(32, 0);
        BLYNK_V_WRITE(V2, 0);
        BLYNK_V_WRITE(V3, 0);
        BLYNK_V_WRITE(V4, 0);
      }
      //digitalWrite(33, Relay3);
      //digitalWrite(32, Relay4);
      //Serial.println("#########################################################################################################################");
    }
  }
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
BLYNK_WRITE(V2) {
  Relay2 = param.asInt();
}
BLYNK_WRITE(V3) {
  Relay3 = param.asInt();
}
BLYNK_WRITE(V4) {
  Relay4 = param.asInt();
}
BLYNK_WRITE(V5) {
  tempset = param.asInt();
}
BLYNK_WRITE(V6) {
  analogset = param.asInt();
}
BLYNK_WRITE(V11) {
  Mode = param.asInt();
}
BLYNK_WRITE(V0) {
  humset = param.asInt();
}
BLYNK_WRITE(V12) {
  TIME_CHECK = param.asInt();
}
BLYNK_WRITE(V13) {
  TIME_RUN = param.asInt();
}
int WifiSignal() {
  //หาค่าสัญญาณ RSSI ของ WIFI แล้ว MAP ให้อยู่ในรูปของเปอร์เซ็นต์
  int CurrentWiFiSignal = WiFi.RSSI();
  if (CurrentWiFiSignal > -40) CurrentWiFiSignal = -40;
  if (CurrentWiFiSignal < -90) CurrentWiFiSignal = -90;
  int WifiSignal = map(CurrentWiFiSignal, -90, -40, 0, 100);
  return WifiSignal;
}
