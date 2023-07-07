#include <ESP8266WiFi.h>

#include <Time.h>



const char* ssid = "FILM69";  //  your network SSID (name)

const char* password = "00000000";       // your network password

//const String month_name[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
//const String day_name[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

const String month_name[12] = {"เดือนมกราคม", "เดือนกุมภาพันธ์", "เดือนมีนาคม", "เดือนเมษายน", "เดือนพฤษภาคม", "เดือนมิถุนายน", "เดือนกรกฎาคม", "เดือนสิงหาคม", "เดือนกันยายน", "เดือนตุลาคม", "เดือนพฤศจิกายน", "เดือนธันวาคม"};

const String day_name[7] = {"วันอาทิตย์", "วันจันทร์ ", "วันอังคาร", "วันพุธ", "วันพฤหัสบดี", "วันศุกร์", "วันเสาร์"};



int timezone = 7 * 3600; //ตั้งค่า TimeZone ตามเวลาประเทศไทย

int dst = 0; //กำหนดค่า Date Swing Time



void setup()

{

  Serial.begin(115200);

  Serial.setDebugOutput(true);



  WiFi.mode(WIFI_STA); //เชื่อมต่อ Wifi

  WiFi.begin(ssid, password);

  Serial.println("\nConnecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("*");

    delay(1000);
  }
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov"); //ดึงเวลาจาก Server

  Serial.println("\nLoading time");

  while (!time(nullptr)) {

    Serial.print("*");

    delay(1000);

  }
  Serial.println("");

}



void loop()

{

  //แสดงเวลาปัจจุบัน

  time_t now = time(nullptr);

  struct tm* p_tm = localtime(&now);
  String date = "เวลา "+String(p_tm->tm_hour)+":"+String(p_tm->tm_min)+":"+String(p_tm->tm_sec)+" "+day_name[(p_tm->tm_wday)]+" ที่ "+String(p_tm->tm_mday)+" "+month_name[(p_tm->tm_mon)]+" พ.ศ. "+String(p_tm->tm_year + 2443);
  Serial.println(date);
  delay(1000);
}
