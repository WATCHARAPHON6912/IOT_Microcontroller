#include <Arduino.h>
#ifdef ESP32
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "EEPROM.h"
#include <WiFi.h>
AsyncWebServer server(80);
const char* ssidAP = "Esp32";
const char* passwordAP = "";
const char* input_parameter1 = "input_string";
const char* input_parameter2 = "input_integer";
const char* input_parameter3 = "input_float";
String INPUTSSID = "";
String INPUTPASS = "";
String INPUTTOKEN = "";
String S, P, T, Data;

char* auth = "";
char* ssid = "";
char* pass = "";

const char index_html[] =
"<!DOCTYPE HTML><html><head>"
"    <meta name=""viewport"" content=""width=device-width, initial-scale=1"">"
"   <style>#file-input,input{width:90%;height:44px;border-radius:4px;margin:10px auto;font-size:20px}"
"    input{background:#f1f1f1;border:0;padding:0 15px}body{background:#00A4FF;font-family:sans-serif;font-size:14px;color:#000000}"
"    #file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"    #bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#00A4FF;width:0%;height:10px}"
"    form{background:#fff;max-width:500px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#00A4FF;color:#fff;cursor:pointer}"
"    </style>"
"  </head>"
"  <meta charset=""UTF-8"">"
"  <meta http-equiv=""X-UA-Compatible"" content=""IE=edge"">"
"  <meta name=""viewport"" content=""width=device-width, initial-scale=1.0"">"
"  <title>ยินดีต้อนรับ</title>"
"  <body>"
"  <form action=""/get"">"
"    <h1>ตั้งค่าโทเค็นชื่อไวไฟและรหัสผ่านไวไฟ</h1>"
"    <input type=""text"" name=""input_string"" placeholder='ป้อนโทเค็น'>"
"    <br/>"
"    <input type=""text"" name=""input_integer"" placeholder='ป้อนชื่อไวไฟ'>"
"    <br/>"
"    <input type=""Password"" name=""input_float"" placeholder='ป้อนรหัสผ่านไวไฟ'>"
"    <input type=""submit"" value=""ยืนยันการตั้งค่า"">"
"  </form>"
"</body></html>";

const char index_html1[] =
"<!DOCTYPE HTML><html><head>"
"    <meta name=""viewport"" content=""width=device-width, initial-scale=1"">"
"   <style>#file-input,input{width:90%;height:44px;border-radius:4px;margin:10px auto;font-size:20px}"
"    input{background:#b9b8b8;border:0;padding:0 15px}body{background:#00A4FF;font-family:sans-serif;font-size:14px;color:#000000}"
"    #file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"    #bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#00A4FF;width:0%;height:10px}"
"    form{background:#fff;max-width:500px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#00A4FF;color:#fff;cursor:pointer}"
"    </style>"
"    </head>"
"    <meta charset=""UTF-8"">"
"    <meta http-equiv=""X-UA-Compatible"" content=""IE=edge"">"
"    <meta name=""viewport"" content=""width=device-width, initial-scale=1.0"">"
"    <title>ยินดีต้อนรับ</title>"
"    <body>"
"    <form action=""/get"">"
"      <h1>ตั้งค่าโทเค็นชื่อไวไฟและรหัสผ่านไวไฟสำเร็จ</h1>"
"    </form>"
"  </body></html>";

void Connect() {

  Data = EEPROM.readString(3);
  Serial.println(Data);
  char auth[EEPROM.read(0) + 1];
  char ssid[EEPROM.read(1) + 1];
  char pass[EEPROM.read(2) + 1];
  for (int a = 0; a < EEPROM.read(0); a++) {
    T += Data[a];
  }
  for (int b = EEPROM.read(0); b < EEPROM.read(0) + EEPROM.read(1); b++) {
    S += Data[b];
  }
  for (int c = EEPROM.read(0) + EEPROM.read(1); c < EEPROM.read(0) + EEPROM.read(1) + EEPROM.read(2); c++) {
    P += Data[c];
  }
  T.toCharArray(auth, EEPROM.read(0)+1);
  S.toCharArray(ssid, EEPROM.read(1)+1);
  P.toCharArray(pass, EEPROM.read(2)+1);

  //Serial.println(auth);
  //Serial.println(ssid);
  //Serial.println(pass);




  //Serial.println(T);
  //Serial.println(P);
  //Blynk.begin(auth, ssid, pass, "oasiskit.com", 8080);
  Blynk.begin(auth, ssid, pass, "oasiskit.com", 8080);
  T = ""; P = ""; S = "";

}
void setwifi() {
  WiFi.softAP(ssidAP, passwordAP);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  //Serial.println(EEPROM.read(0));
  //Serial.println(EEPROM.read(1));
  //Serial.println(EEPROM.read(2));
  //Serial.println("END Read");
  //Data = EEPROM.readString(3);
  //Serial.println(Data);
  /*
    for (int a = 0; a < EEPROM.read(0); a++) {
    T += Data[a];
    }
    for (int b = EEPROM.read(0); b < EEPROM.read(0) + EEPROM.read(1); b++) {
    S += Data[b];
    }
    for (int c = EEPROM.read(0) + EEPROM.read(1); c < EEPROM.read(0) + EEPROM.read(1) + EEPROM.read(2); c++) {
    P += Data[c];
    }


    Serial.println(T);
    Serial.println(S);
    Serial.println(P);
    S = ""; P = ""; T = "";
    /*
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connecting...");
    return;
    }

    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  */

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String input_message;
    String input_parameter;

    if (request->hasParam(input_parameter1)) {
      INPUTTOKEN = request->getParam(input_parameter1)->value();
      INPUTSSID = request->getParam(input_parameter2)->value();
      INPUTPASS = request->getParam(input_parameter3)->value();
      input_parameter = input_parameter3;
    }
    else {
      input_message = "No message sent";
      input_parameter = "none";
    }

    EEPROM.write(1, INPUTSSID.length());
    EEPROM.write(2, INPUTPASS.length());
    EEPROM.write(0, INPUTTOKEN.length());
    EEPROM.writeString(3, INPUTTOKEN +  INPUTSSID +  INPUTPASS);
    EEPROM.commit();

    Serial.println(INPUTSSID.length());
    Serial.println(INPUTSSID);
    Serial.println(INPUTPASS.length());
    Serial.println(INPUTPASS);
    Serial.println(INPUTTOKEN.length());
    Serial.println(INPUTTOKEN);
    request->send(200, "text/html",index_html1);
    ESP.restart();
  });

  //server.onNotFound(notFound);
  server.begin();
}
