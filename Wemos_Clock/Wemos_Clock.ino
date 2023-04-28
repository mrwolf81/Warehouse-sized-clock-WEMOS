
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
//#include "DHT.h"
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include "secrets.h"
//#include "Home.h"
#include "Seg_Display.h"


//#define DHTPIN D5
//#define DHTTYPE DHT11
//#define ntpServerName "au.pool.ntp.org"
//#define timeZone "AEST-10AEDT,M10.0,M4.1.0/3"

///DHT dht(DHTPIN, DHTTYPE);  //DHT_Unified
ESP8266WebServer server(80);

//const char ssid[] = "STASSID";  //  your network SSID (name)
//const char pass[] = "STAPSK";


// NTP Servers:
//static const char ntpServerName[] = "au.pool.ntp.org";
//const int timeZone = +10;


//WiFiUDP Udp;
//unsigned int localPort = 8888;

//time_t getNtpTime();
//time_t prevDisplay = 0;
//void printDigits(int digits);
//void sendNTPpacket(IPAddress &address);

// when the digital clock was displayed
byte g_digits [10];                  // Definitions of the 7-bit values for displaying digits
byte g_digits2 [10];
int g_numberToDisplay = 0;          // default number being displayed, 0 
const int g_registers = 4;          // Number of shift registers in use, 4
byte g_registerArray [g_registers]; // Array of numbers to pass to shift registers


const int g_pinData = D3;         //D1
const int g_pinCommLatch = D2;    //D2
const int g_pinClock = D1;        //D3
const int dot1 = D7;
const int dot2 = D8;
//int temp = 0;
//int humid = 0;




void setup(){

int a = 1, b = 2, c = 4, d = 8, e = 16, f = 32, g = 64;

    g_digits [0] = 8 + 4 + 2 + 64 + 32 + 1;             //1 + 2 + 4 + 8 + 16 + 32;
    g_digits [1] = 4 + 2;                               //2 + 4;
    g_digits [2] = 8 + 4 + 16 + 32 + 64;                //1 + 2 + 64 + 16 + 8;
    g_digits [3] = 8 + 4 + 16 + 2 + 64;                 //1 + 2 + 4 + 8 + 64;
    g_digits [4] = 1 + 16 + 4 + 2;                      //32 + 64 + 2 + 4;
    g_digits [5] = 8 + 1 + 16 + 2 + 64;                 //1 + 32 + 64 + 4 + 8;
    g_digits [6] = 8 + 1 + 16 + 2 + 64 + 32;            //1 + 32 + 16 + 8 + 4 + 64;
    g_digits [7] = 8 + 4 + 2;                           //1 + 2 + 4;
    g_digits [8] = 1 + 2 + 4 + 8 + 16 + 32 + 64;        //1 + 2 + 4 + 8 + 16 + 32 + 64;
    g_digits [9] = 1 + 8 + 4 + 16 + 2 + 64;             //8 + 4 + 2 + 1 + 32 + 64;
    g_digits2 [0] = 1 + 8 + 4 + 16;                     //1 + 2 + 64 + 32;           // Degree dot
    g_digits2 [1] = 8 + 1 + 32 + 64;                    //1 + 8 + 16 + 32;           // Capital C
    g_digits2 [2] = 32 + 16;                            //64 + 16;                   //  r, 80
    g_digits2 [3] = 1 + 32 + 16 + 2;                    //4 + 16 + 32 + 64;           //  h, 116
    g_digits [99] = 0;
/*
    for (int i=0;i<5;i++) {
  
    g_registerArray [0] = g_digits [8];
    g_registerArray [1] = g_digits [8];
    g_registerArray [2] = g_digits [8];
    g_registerArray [3] = g_digits [8];
   
    sendSerialData (g_registers, g_registerArray);
    delay(200);

    g_registerArray [0] = g_digits [99];
    g_registerArray [1] = g_digits [99];
    g_registerArray [2] = g_digits [99];
    g_registerArray [3] = g_digits [99];
   
    sendSerialData (g_registers, g_registerArray);
    delay(200);
}*/
}

/*
Serial.begin(115200);
  while (!Serial) ; // Needed for Leonardo only
  delay(250);
  Serial.println("Warehouse Clock");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, passPhrase);
  //dht.begin();
  //configTime(timeZone, ntpServerName);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
/*
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
*/
void loop(){
  Seg_Display;
 /* if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      if ((second() == 00) || (second() == 15) ||(second() == 30) ||(second() == 45)){
        Seg_Display_Temp;
        delay (100);
        Seg_Display_Humid;
        delay (200);
      }else{
        Seg_Display;
      }
    }
  }*/

}



/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
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
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
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

void handle_OnConnect() {
  struct tm timeinfo;

  temp = temp,DEC; // Gets the values of the temperature
  humid = dht.readHumidity(); // Gets the values of the humidity
  timeH = hour();
  timeM = minute();
  server.on("/$upload.htm",[](){
  server.send(200, "text/html", FPSTR(uploadContent));
  //server.send(200, "text/html", SendHTML(temp,humid,timeH,timeM));
});
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

//String SendHTML(float tempstat,float humidstat,float timeHstat,float timeMstat);
