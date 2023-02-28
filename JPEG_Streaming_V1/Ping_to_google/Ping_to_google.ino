#include <WiFi.h>

//the wi-fi credentials are below
const char* ssid = "AndroidAP";
const char* pass = "qxom6613";
const char* host = "www.google.com";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, pass);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;

  if (!client.connect(host, 80)) {
    Serial.println("Connection failed");
    delay(1000);
    return;
  }

  // Send a HTTP request
  client.print(String("GET / HTTP/1.1\r\n") +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long start_time = millis();

  // Wait for the server to respond
  while (client.connected() && !client.available()) {
    delay(1);
  }

  unsigned long end_time = millis();

  // Measure the round-trip time
  int rtt = end_time - start_time;

  Serial.print("RTT: ");
  Serial.print(rtt);
  Serial.println(" ms");

  delay(1000);
}
