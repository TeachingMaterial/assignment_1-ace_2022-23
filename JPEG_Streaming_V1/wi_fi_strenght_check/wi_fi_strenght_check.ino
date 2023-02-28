//The following scketch is used to check the wifi strength
// from -127 (weak) to -1dB (strong) as an int
#include "WiFi.h"

//the wi-fi credentials are below
const char* ssid = "AndroidAP";
const char* pass = "qxom6613";


void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  delay(100);
}

void loop(){
  int sig = WiFi.RSSI();
  //print the wi-fi strenght to the serial monitor
  Serial.print("Wi-Fi strenght: ");
  Serial.println(sig);
  Serial.print(" dB");
  delay(2000);
}
