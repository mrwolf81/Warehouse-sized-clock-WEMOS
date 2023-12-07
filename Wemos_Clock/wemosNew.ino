#include "SegDisplay.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "RTClib.h"
#include <TimeLib.h>
#include <MyTimezone.h>
#include <Wire.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT11

RTC_Millis RTC;

static const char ntpServerName[] = "au.pool.ntp.org";
WiFiUDP Udp;
uint16_t localPort;

TimeElements myTimeElements;

SegDisplay SegDisplay(D4,D0,D3);  //D2,D3,D4

AsyncWebServer server(80);

const int dot1 = D7;                //colon dot
const int dot2 = D8;                //colon dot 2 / AM/PM light
//const int dot3 = D4;              //AM/PM light constantly on
const int outputEnablePin = D6;     //brightness control
DHT dht(DHTPIN, DHTTYPE);

TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
Timezone ausET(aEDT, aEST);
TimeChangeRule *tcr;

//AEST-10AEDT,M10.1.0,M4.1.0/3  //another timezone setting that was not used

int status = WL_IDLE_STATUS;
float temp1 = 0.0;
float humid1 = 0.0;
int temp = 0;
int humid = 0;
int ampm;

//--------------------------------- webserver params ---------------------------------------------------------//
String sliderValue = "0";
String DateTimeUpdate = "0";
String updRTC = "0";
const char*PARAM_INPUT = "value";
const char*PARAM_INPUT_DATETIME = "datetimeRTC";
const char*PARAM_INPUT_SYNC = "sync";
const char*PARAM_INPUT_ALARM1 = "alarm1";
const char*PARAM_INPUT_ALARM2 = "alarm2";
const char*PARAM_INPUT_ALARM3 = "alarm3";
const char*PARAM_INPUT_ALARM4 = "alarm4";
const char*SSID_1 = "ssid";
const char*PASSWORD = "password";
const char*REBOOT = "reboot";

//-------------------------- string functions for displaying info on webserver pages -------------------------//

String getMyTime(){
  time_t utc = now();
  time_t prevDisplay = 0;
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      tmElements_t tm;
      breakTime(now(), tm);
      utc = now();
    }
  }
  char timebuf[6];
  sprintf(timebuf,"%02d:%02d:%02d",hourFormat12(ausET.toLocal(utc, &tcr)),minute(),second(),ampm);
  String timebuffer = timebuf;
  return String(timebuffer);
}

String getRTCTime(){
  DateTime dt = RTC.now();
  int rtc_hour = dt.hour();
  int am_pm;
  if(dt.hour() == 0){
      rtc_hour += 12;
      am_pm = 1; 
    }else if(dt.hour() > 12){
      rtc_hour -= 12;
      am_pm = 0;
    }
  char timebuf[6];
  sprintf(timebuf,"%02d:%02d:%02d",rtc_hour,dt.minute(),dt.second());
  String timebuffer = timebuf;
  return String(timebuffer);
}

String getRTCDate(){
  DateTime dt = RTC.now();
  char datebuff[8];
  sprintf(datebuff, "%02d-%02d-%04d",dt.day(),dt.month(),dt.year());
  String datebuffers = datebuff;
  return String(datebuffers);
}


String getMyDate(){
  char datebuf[8];
  sprintf(datebuf,"%02d-%02d-%04d",day(),month(),year());
  String datebuffer = datebuf;
  return String(datebuffer);
}

String getAMPM(){
  time_t utc = now();
  time_t t = ausET.toLocal(utc, &tcr);
  String AMPM[2]={"AM","PM"};
  String ampm = AMPM[isPM(t)];
  return String(ampm);
}

String getRTCAMPM(){
  DateTime dt = RTC.now();
  int am_pm;
  if(dt.hour() < 12){ //am
      am_pm = 0; 
    }else if(dt.hour() >= 12){
      am_pm = 1;   //pm
    }
  String AMPM[2]={"AM","PM"};
  String RTCampm = AMPM[am_pm];
  return String(RTCampm);
}

String getNowTime(){
  time_t utc = now();
  time_t prevDisplay = 0;
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      tmElements_t tm;
      breakTime(now(), tm);
      utc = now();
    }
  }
  char timebuf[4];
  sprintf(timebuf,"%02d:%02d",hour(ausET.toLocal(utc, &tcr)),minute());
  String timebuffer = timebuf;
  return String(timebuffer);
}

String alarmtime1(){
  String alarmtime = readFile(SPIFFS,"/ALARM1.txt");
  while (Serial.available()>0){
    int inChar = Serial.read();
    if(isDigit(inChar)){
      alarmtime +=(char)inChar;
      }
      if(inChar == '\n'){
        Serial.print("value :");
        Serial.println(alarmtime.toInt());
        Serial.print("string :");
        Serial.println(alarmtime);
        }
     }
  return String(alarmtime);
}

String alarmtime2(){
  String alarmtime = readFile(SPIFFS,"/ALARM2.txt");
  while (Serial.available()>0){
    int inChar = Serial.read();
    if(isDigit(inChar)){
      alarmtime +=(char)inChar;
      }
      if(inChar == '\n'){
        Serial.print("value :");
        Serial.println(alarmtime.toInt());
        Serial.print("string :");
        Serial.println(alarmtime);
        }
     }
  return String(alarmtime);
}

String alarmtime3(){
  String alarmtime = readFile(SPIFFS,"/ALARM3.txt");
  while (Serial.available()>0){
    int inChar = Serial.read();
    if(isDigit(inChar)){
      alarmtime +=(char)inChar;
      }
      if(inChar == '\n'){
        Serial.print("value :");
        Serial.println(alarmtime.toInt());
        Serial.print("string :");
        Serial.println(alarmtime);
        }
     }
  return String(alarmtime);
}

String alarmtime4(){
  String alarmtime = readFile(SPIFFS,"/ALARM4.txt");
  while (Serial.available()>0){
    int inChar = Serial.read();
    if(isDigit(inChar)){
      alarmtime +=(char)inChar;
      }
      if(inChar == '\n'){
        Serial.print("value :");
        Serial.println(alarmtime.toInt());
        Serial.print("string :");
        Serial.println(alarmtime);
        }
     }
  return String(alarmtime);
}

//------------------- function to read data from saved files "SPIFFS" ---------------------------//

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  Serial.println(fileContent);
  return fileContent;
}

//-------------------- function to write info to saved files "SPIFFS" -------------------------------//

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

//----------------------------------- Replaces placeholder on web page -------------------------------//

String processor(const String& var){
  Serial.println(var);
  if(var == "TIME"){
    return getMyTime();
  }
  else if(var == "DATE"){
    return getMyDate();
  }
  else if(var == "TIMERTC"){
    return getRTCTime();
  }
  else if(var == "DATERTC"){
    return getRTCDate();
  }
  else if(var == "TEMPERATURE"){
    return String(temp1);
  }
  else if(var == "HUMIDITY"){
    return String(humid1);
  }
  else if(var == "AMPM"){
    return getAMPM();
  }
  else if(var == "AMPMRTC"){
    return getRTCAMPM();
  }
  else if(var == "SLIDERVALUE"){
    return sliderValue;
  }
  else if(var == "SSID"){
    return readFile(SPIFFS,"/SSID.txt");
  }
  else if(var == "PASSWORD"){
    return readFile(SPIFFS,"/PASS.txt");
  }
  else if(var == "ALARM1"){
    return readFile(SPIFFS,"/ALARM1.txt");
  }
  else if(var == "ALARM2"){
    return readFile(SPIFFS,"/ALARM2.txt");
  }
  else if(var == "ALARM3"){
    return readFile(SPIFFS,"/ALARM3.txt");
  }
  else if(var == "ALARM4"){
    return readFile(SPIFFS,"/ALARM4.txt");
  }
  return String();
}

//---------------- Brightness Control (work in progress) ----------------------//

void setBrightness(byte brightness){
  analogWrite(outputEnablePin, 255-brightness);
}

//------------------------ Main Setup -----------------------------------------//
void setup(){
  Serial.begin(115200);
  Wire.begin();
  dht.begin();
  pinMode (dot1, OUTPUT);
  pinMode (dot2, OUTPUT); 
  pinMode(outputEnablePin, OUTPUT);
  localPort = random(1024,65535);
  Udp.begin(localPort);
  #ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
    #endif

    // following line sets the RTC to the date & time this sketch was compiled
    RTC.begin(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));*/
  
  if(!SPIFFS.begin()){          //Initialize SPIFFS
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  String _ssid = readFile(SPIFFS,"/SSID.txt");
  String _pass = readFile(SPIFFS,"/PASS.txt");
  
  const char * hostName = "LEDWIFICLOCKII";
  const char * hostPass = "noSpoon";
  const char* http_username = "admin";
  const char* http_password = "admin";
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(hostName);
  WiFi.begin(_ssid.c_str(),_pass.c_str());
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("STA: Failed!\n");
    WiFi.disconnect(false);
    delay(1000);
    SegDisplay.blinkDisplay();
  }
//------------------------------------ load web pages -------------------------------------------//
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", String(), false, processor);
      });
    
    server.on("/clocksettings.html", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/clocksettings.html", String(), false, processor);
      });
  
    server.on("/alarmsettings.html", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS,"/alarmsettings.html", String(), false, processor);
      });
  
    server.on("/firmware.html", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/firmware.html", String(), false, processor);
      });
  
    server.on("/wifisettings.html", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/wifisettings.html", String(), false, processor);
      });
  
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
      });
  
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/script.js", "text/js");
      });

  //---------------------- Get info to send to webpages -------------------------//
  
  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", getMyTime().c_str());
  });

  server.on("/timertc", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", getRTCTime().c_str());
  });

  server.on("/timertc", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", getRTCDate().c_str());
  });
  
  server.on("/date", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", getMyDate().c_str());
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", String(temp1).c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", String(humid1).c_str());
  });

//---------------------- Get inputs from webpages -------------------------//

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      setBrightness(sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);

    request->send(200, "text/plain", "OK");

  });
  
  //-------------------- RTC update request via webserver input ----------------------//
  
  server.on("/updatertc", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_DATETIME)) {
      inputMessage = request->getParam(PARAM_INPUT_DATETIME)->value();
      inputParam = PARAM_INPUT_DATETIME;
      updRTC = inputMessage;
      updRTC.toInt();
    }
    
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.print("actual value entered : ");
    Serial.println(inputMessage);
    request->redirect("/clocksettings.html");
  });

//------------------ sync RTC with NTP via webserver button ---------------------------//

  server.on("/syncrtc", HTTP_GET, [] (AsyncWebServerRequest *request){
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_SYNC)) {
      inputMessage = request->getParam(PARAM_INPUT_SYNC)->value();
      inputParam = PARAM_INPUT_SYNC;
      time_t utc = now();
      time_t t = ausET.toLocal(utc, &tcr);
      RTC.adjust(DateTime(year(t),month(t),day(t),hour(t),minute(t),second(t)));
    }
    else{
      inputMessage = "no message sent";
      inputParam = "none";
    }
    request->redirect("/clocksettings.html");
  });

//---------------- alarm inputs via webserver selection -----------------------------------//

  server.on("/alarm", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_ALARM1)) {
      inputMessage = request->getParam(PARAM_INPUT_ALARM1)->value();
      writeFile(SPIFFS,"/ALARM1.txt",inputMessage.c_str());
      inputParam = PARAM_INPUT_ALARM1;
    }else
    if (request->hasParam(PARAM_INPUT_ALARM2)) {
      inputMessage = request->getParam(PARAM_INPUT_ALARM2)->value();
      writeFile(SPIFFS,"/ALARM2.txt",inputMessage.c_str());
      inputParam = PARAM_INPUT_ALARM2;
    }else
    if (request->hasParam(PARAM_INPUT_ALARM3)) {
      inputMessage = request->getParam(PARAM_INPUT_ALARM3)->value();
      writeFile(SPIFFS,"/ALARM3.txt",inputMessage.c_str());
      inputParam = PARAM_INPUT_ALARM3;
    }else
    if (request->hasParam(PARAM_INPUT_ALARM4)) {
      inputMessage = request->getParam(PARAM_INPUT_ALARM4)->value();
      writeFile(SPIFFS,"/ALARM4.txt",inputMessage.c_str());
      inputParam = PARAM_INPUT_ALARM4;
    }else
      {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->redirect("/alarmsettings.html");
  });

//------------------- SSID and Paswword inputs from webserver input ---------------------------//

  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    String inputMessage;
    String inputMessagePass;
    String inputParam;
    if (request->hasParam(SSID_1) && request->hasParam(PASSWORD)){
      inputMessage = request->getParam(SSID_1)->value();
      inputMessagePass = request->getParam(PASSWORD)->value();
      inputParam = SSID_1;
      writeFile(SPIFFS,"/SSID.txt",inputMessage.c_str());
      inputParam = PASSWORD;
      writeFile(SPIFFS,"/PASS.txt",inputMessagePass.c_str());
    }
    else{
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    Serial.println(inputMessagePass);
    request->redirect("/wifisettings.html");
  });

//--------------- reset function (needs work) ----------------------------------------------------//
  
  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
    String inputMessage;
    String inputParam;
    if (request->hasParam(REBOOT)){
      delay(100);
      ESP.reset();
    }
    else{
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
  });
 
  // Start server
  server.begin();
}

time_t prevDisplay = 0; // previous time info set to zero, needed to clear old info for new

//--------------------------- Loop - display time and date from NTP if connected ---------------------------------------------//
void loop(){
   if (WiFi.status() == WL_CONNECTED){
    setSyncProvider(getNtpTime);
    setSyncInterval(60);
    Serial.println("CONNECTED");
    time_t utc = now();
    time_t t = ausET.toLocal(utc, &tcr);

    float newT = dht.readTemperature();
    temp1 = newT;
    float newH = dht.readHumidity();
    humid1 = newH;
    
    if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
        Serial.print("NTP time: ");
        Serial.print(hour(t));
        Serial.print(":");
        Serial.print(minute(t));
        Serial.print(":");
        Serial.print(second(t));
        Serial.print(" ");
        if(isAM(t) == true){
          Serial.println("am");
        }else{
          Serial.println("pm");
        }
        DateTime dt = RTC.now();
        int rtc_hour = dt.hour();
        int am_pm;
        if(dt.hour() == 0){
          rtc_hour += 12;
          am_pm = 1; 
        }else if(dt.hour() > 12){
          rtc_hour -= 12;
          am_pm = 0;
         }
        Serial.print("RTC time : ");
        Serial.print(dt.hour());
        Serial.print(":");
        Serial.print(dt.minute());
        Serial.print(":");
        Serial.print(dt.second());
        if(am_pm == 1){
          Serial.println("am");
        }else{
          Serial.println("pm");
        }
        Serial.print(dt.day());
        Serial.print("-");
        Serial.print(dt.month());
        Serial.print("-");
        Serial.println(dt.year());
        
      if((second() == 00) || (second() == 15) || (second() == 30) || (second() == 45)){
        SegDisplay.DisplayTemp(newT,newT);
        delay(1000);
        SegDisplay.DisplayHumid(newH,newH);
        delay(1000);
        SegDisplay.DisplayDate(day(),day(),month(),month());
        delay(1000);
        SegDisplay.DisplayYear(year());
        delay(500);
      }else{
        SegDisplay.updateDisplay(hourFormat12(ausET.toLocal(utc, &tcr)),hourFormat12(ausET.toLocal(utc, &tcr)),minute(),minute());
      }
      }
      }

//----------------------------- Display Time & Date via RTC if netwrok not available or disconnected --------------------------------------//
      
  }else if (WiFi.status() == WL_DISCONNECTED || WL_NO_SSID_AVAIL || WL_CONNECT_FAILED || WL_CONNECTION_LOST){
    DateTime dt = RTC.now();
    int rtc_hour = dt.hour();
    int am_pm;
    if(dt.hour() == 0){
      rtc_hour += 12; 
    }else if(dt.hour() > 12){
      rtc_hour -= 12;
    }
    Serial.println(dt.hour());
    Serial.println("DISCONNECTED");
    Serial.print("RTC time: ");
    Serial.print(rtc_hour);
    Serial.print(":");
    Serial.print(dt.minute());
    Serial.print(":");
    Serial.print(dt.second());
    if(dt.hour() < 12){
      digitalWrite(dot2, HIGH);
      Serial.println("am");
    }else if(dt.hour() >= 12){
      digitalWrite(dot2, LOW);
      Serial.println("pm");
    }
    Serial.print("RTC date: ");
    Serial.print(dt.day());
    Serial.print("-");
    Serial.print(dt.month());
    Serial.print("-");
    Serial.println(dt.year());
    float newT = dht.readTemperature();
    temp1 = newT;
    float newH = dht.readHumidity();
    humid1 = newH;
      if((second() == 00) || (second() == 15) || (second() == 30) || (second() == 45)){
        SegDisplay.DisplayTemp(newT,newT);
        delay(1000);
        SegDisplay.DisplayHumid(newH,newH);
        delay(1000);
        SegDisplay.DisplayDate(day(),day(),month(),month());
        delay(1000);
        SegDisplay.DisplayYear(year());
        delay(500);
      }else{
        SegDisplay.updateDisplay(rtc_hour,rtc_hour,dt.minute(),dt.minute());
      }
  }else{
    Serial.print("unknown status : ");
    Serial.println(WiFi.status());
  }
  analogWrite(dot1, 120);
  delay(500);
  analogWrite(dot1, LOW);
  delay(500);

//------------------- Function to update the RTC via input from the webserver -------------------------------------//
  
  if(updRTC.toInt() > 0){
    Serial.print("updRTC value :");
    Serial.println(updRTC);
    int _hour = 0;
    int _minute = 0;
    int _second = 0;
    int _day = 0;
    int _month = 0;
    int _year = 0;
    char string[17];
    int str_len = updRTC.length()+1;
    char char_array[str_len];
    updRTC.toCharArray(char_array,str_len);
    sscanf(char_array, "%04d-%02d-%02dT%02d:%02d",&_year,&_month,&_day,&_hour,&_minute);
    Serial.print("conversion : ");
    Serial.print(_day);
    Serial.print("-");
    Serial.print(_month);
    Serial.print("-");
    Serial.print(_year);
    Serial.print(" ");
    Serial.print(_hour);
    Serial.print(":");
    Serial.println(_minute);
    RTC.adjust(DateTime(_year,_month,_day,_hour,_minute,0));
    delay(60);
    updRTC = 0;
  }else{
    Serial.println("no rtc update info");
  }
}

//---------------------------- function to get the current date and time from network server ----------------------------------------//

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  //Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  //Serial.print(ntpServerName);
  //Serial.print(": ");
  //Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

//----------------- send an NTP request to the time server at the given address------------//

void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
