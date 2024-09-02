#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_I2CDevice.h>
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library

#define CLK 27
#define DT 26
#define SW 25
#define upBTN 13
#define downBTN 12
#define SLIDER 32

#define TFT_MOSI 23  // SDA Pin on ESP32
#define TFT_SCLK 18  // SCL Pin on ESP32
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin)

// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
 
float p = 3.1415926;

volatile int16_t position = 0;
volatile int16_t lastCLKState;
volatile bool positionChanged = false;

int lastSLIDER = -1;
bool lastSW = HIGH;
bool lastUpBTN = LOW;
bool lastDownBTN = LOW;
int lastMappedSlider = -1;

void tftPrintTest() {

}


void IRAM_ATTR handleEncoder() {
  int16_t currentCLKState = digitalRead(CLK);
  if (currentCLKState != lastCLKState) {
    if (digitalRead(DT) != currentCLKState) {
      position++;
    } else {
      position--;
    }
    positionChanged = true;
  }
  lastCLKState = currentCLKState;
}

void printSerial(int mappedSlider, bool currentSW, bool currentUpBTN, bool currentDownBTN, int position) {
  Serial.print(mappedSlider);
  Serial.print(",");
  Serial.print(currentSW);
  Serial.print(",");
  Serial.print(currentUpBTN);
  Serial.print(",");
  Serial.print(currentDownBTN);
  Serial.print(",");
  Serial.println(position);

  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.println("Slider Value:");
  tft.println(mappedSlider);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Rotary Decoder Switch Status:");
  tft.println(currentSW);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.println("Up Button Status:");
  tft.println(currentUpBTN);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(2);
  tft.println("Down Button Status:");
  tft.println(currentDownBTN);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.println("Rotary Decoder: ");
  tft.println(position);
}

void setup() {
  Serial.begin(115200);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP); // Set the switch pin as input with pull-up
  pinMode(upBTN, INPUT);
  pinMode(downBTN, INPUT);
  pinMode(SLIDER, INPUT);
  lastCLKState = digitalRead(CLK);
  attachInterrupt(digitalPinToInterrupt(CLK), handleEncoder, CHANGE);

  tft.init(240, 280, SPI_MODE2);    // Init ST7789 display 135x240 pixel
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.invertDisplay(true);
}

void loop() {
  bool currentSW = digitalRead(SW) == LOW;
  bool currentUpBTN = digitalRead(upBTN) == HIGH;
  bool currentDownBTN = digitalRead(downBTN) == HIGH;
  int currentSLIDER = analogRead(SLIDER);
  int mappedSlider = map(currentSLIDER, 0, 4095, 0, 100.0);

  // Print if the position has changed
  if (positionChanged) {
    printSerial(mappedSlider, currentSW, currentUpBTN, currentDownBTN, position);
    positionChanged = false;
  }

  // Print if the slider value has changed
  if (mappedSlider != lastSLIDER) {
    if (abs(mappedSlider - lastSLIDER) >= 2){
      printSerial(mappedSlider, currentSW, currentUpBTN, currentDownBTN, position);
      lastSLIDER = mappedSlider;
    }
  }

  // Print if the switch state has changed
  if (currentSW != lastSW) {
    printSerial(mappedSlider, currentSW, currentUpBTN, currentDownBTN, position);
    lastSW = currentSW;
  }

  // Print if the up button state has changed
  if (currentUpBTN != lastUpBTN) {
    printSerial(mappedSlider, currentSW, currentUpBTN, currentDownBTN, position);
    lastUpBTN = currentUpBTN;
  }

  // Print if the down button state has changed
  if (currentDownBTN != lastDownBTN) {
    printSerial(mappedSlider, currentSW, currentUpBTN, currentDownBTN, position);
    lastDownBTN = currentDownBTN;
  }

  delay(100); // Debounce delay
}
