// Adafruit_ImageReader test for Adafruit ST7735 TFT Breakout for Arduino.
// Demonstrates loading images from SD card or flash memory to the screen,
// to RAM, and how to query image file dimensions.
// Requires three BMP files in root directory of SD card:
// rgbwheel.bmp, miniwoof.bmp and wales.bmp.
// As written, this uses the microcontroller's SPI interface for the screen
// (not 'bitbang') and must be wired to specific pins (e.g. for Arduino Uno,
// MOSI = pin 11, MISO = 12, SCK = 13). Other pins are configurable below.

#include <Adafruit_GFX.h>         // Core graphics library
#include <Adafruit_SSD1331.h>      // Hardware-specific library
#include <SdFat.h>                // SD card & FAT filesystem library
#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
#include <Adafruit_ImageReader.h> // Image-reading functions

// Comment out the next line to load from SPI/QSPI flash instead of SD card:
#define USE_SD_CARD

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

// TFT display and SD card share the hardware SPI interface, using
// 'select' pins for each to identify the active device on the bus.

#define TFT_CS  5 // TFT select pin
#define TFT_RST  -1 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC   3 // TFT display/command pin
#define SD_CS    5 // SD card select pin
#define TFT_MOSI  23
#define TFT_MISO  19
#define TFT_SCK 18


#if defined(USE_SD_CARD)
  SdFat                SD;         // SD card filesystem
  Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys
#else
  // SPI or QSPI flash filesystem (i.e. CIRCUITPY drive)
  #if defined(__SAMD51__) || defined(NRF52840_XXAA)
    Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS,
      PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
  #else
    #if (SPI_INTERFACES_COUNT == 1)
      Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
    #else
      Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
    #endif
  #endif
  Adafruit_SPIFlash    flash(&flashTransport);
  FatVolume        filesys;
  Adafruit_ImageReader reader(filesys); // Image-reader, pass in flash filesys
#endif


Adafruit_SSD1331 tft = Adafruit_SSD1331(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);

void setup(void) {


  ImageReturnCode stat; // Status from image-reading functions

  Serial.begin(115200);

  Serial.println(F("Initializing screen"));
  tft.begin(); // Initialize screen

  // Fill screen blue. Not a required step, this just shows that we're
  // successfully communicating with the screen.
  Serial.println("Filling screen BLUE");
  tft.fillScreen(BLUE);
  Serial.println("Screen fill complete");

  
  // The Adafruit_ImageReader constructor call (above, before setup())
  // accepts an uninitialized SdFat or FatVolume object. This MUST
  // BE INITIALIZED before using any of the image reader functions!
  Serial.print(F("Initializing filesystem..."));

  // SD card is pretty straightforward, a single call...
  if(!SD.begin(SD_CS, SD_SCK_MHZ(10))) { // Breakouts require 10 MHz limit due to longer wires
    Serial.println(F("SD begin() failed"));
    for(;;); // Fatal error, do not continue
  }

  Serial.println(F("OK!"));


// Load full-screen BMP file 'daffodil.bmp' at position (0,0) (top left).
// Notice the 'reader' object performs this, with 'tft' as an argument.
  Serial.print(F("Loading daffodil.bmp to screen..."));
  stat = reader.drawBMP("/daffodil.bmp", tft, 0, 0);
  reader.printStatus(stat);   // How'd we do?

//Process to read image files from SD card and write to display
//  int fileNumber = 1;
//  String fileNamePrefix = "/IMG_";
//  String fileNameSuffix = ".bmp";
//
//  for (fileNumber; fileNumber < 4; fileNumber++){
//    String strFileNumber = String(fileNumber);
//    String fileName = fileNamePrefix + strFileNumber + fileNameSuffix;
//    Serial.println(fileName);
//  // Load full-screen BMP file 'daffodil.bmp' at position (0,0) (top left).
//  // Notice the 'reader' object performs this, with 'tft' as an argument.
//  const char * filePrint = fileName.c_str();
//  Serial.print(F("Loading image to screen..."));
//  stat = reader.drawBMP(filePrint, tft, 0, 0);
//  reader.printStatus(stat);   
//    delay(1000);
//  }
}

void loop() {
  
 setup();
 delay(2000); // Pause 2 sec.

}
