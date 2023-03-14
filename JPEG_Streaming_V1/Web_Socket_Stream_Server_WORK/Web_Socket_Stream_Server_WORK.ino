//The following code is used to stream JPEG frames using Websocket server.
//The images are captured with ESP32-CAM and quality 320x240

//The following Arduino Websocket library was used: https://github.com/gilmaimon/ArduinoWebsockets

#include "esp_camera.h"
#include <WiFi.h>
#include <ArduinoWebsockets.h>

#define CAMERA_MODEL_AI_THINKER

//CAMERA_MODEL_AI_THINKER_PINS
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

using namespace websockets;

WebsocketsServer server;//websocket server 
WebsocketsClient client;//websocket client 
bool check_connection;//used to check the connection

const char* ssid = "AndroidAP";
const char* password = "qxom6613";

//server port number 80
const uint16_t s_port = 80;

//Webcocket callback event ot check for connection status
void ws_event_callback(WebsocketsEvent event, String data){
  //checking the status of the Websocket connection
  if(event == WebsocketsEvent::ConnectionOpened){
       Serial.println("Connection Opened");
       check_connection = true;
    }
    //if the connection failed attempt to connect again
    else if(event == WebsocketsEvent::ConnectionClosed)
    {
       Serial.println("Connection Closed");
       check_connection = false;
  }
}

void setup() {
  check_connection = false;
  //Starting the serial communication
  Serial.begin(115200);
  Serial.println();
  Serial.flush();
  
  //Assigning the camera pins of ESP32-CAM 
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //checking for pseudo RAM
  //important for jpeg frame capture
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA; 
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } 
  else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  //initialisation of 0V2640 camera model configuration
  //checking for succesfull initialisation
  esp_err_t init_error = esp_camera_init(&config);
  if (init_error != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", init_error);
    return;
  }
  
  WiFi.begin(ssid, password);
  
  Serial.print("Wi-Fi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    //waiting for 0.5 sec
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connection established!");

  
  Serial.print("Ready for streaming!");
  Serial.print("Link: ws://");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.print(s_port);
  Serial.println("");
  server.listen(s_port);
  Serial.print("Is the Server available? -> ");
  //Serial.println(server.available());
  if(server.available()!=0){
    Serial.println("Yes");
  }
  else
  { 
    Serial.println("No");
  }
}

void loop() {
  
  if(server.poll()){
     client = server.accept();
  }

  //while the Websocket connection is open stream jpeg frames
  if(client.available()){
     client.poll();

     //getting new frames from the camera
     camera_fb_t *fb = NULL;
     esp_err_t res = ESP_OK;
     fb = esp_camera_fb_get();
     
    //checking of the frame capture was succsessfull
    if(!fb){
       Serial.println("Camera capture failed");
       esp_camera_fb_return(fb);
       return;
    }
    //sending JPEG frames as binary
    client.sendBinary((const char*) fb->buf, fb->len);
    esp_camera_fb_return(fb);
  } 
}
