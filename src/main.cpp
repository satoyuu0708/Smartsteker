// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For red6x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <BLEDevice.h>
#include <BLE2902.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
/*#include <cstdint>
#include <FS.h>
#include <Wire.h>
#include <SPI.h>
#include <SPIFFS.h>*/




// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin WHITEWHITE works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

// USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK WHITEWHITE // USE THIS ON ARDUINO MEGA

#define CLK  15
#define OE   18
#define LAT  19
#define A   12
#define B   16
#define C   17
//#define D   22
#define PIN_R1  25
#define PIN_G1  26
#define PIN_B1  27
#define PIN_R2  21
#define PIN_G2 22
#define PIN_B2 23
#define mw 32
#define mh 16
#define NUMMATRIX (mw*mh)//電光掲示板の面積
#define setBrightness(x) fillScreen(15) // no-op, no brightness on this board
#define clear()          fillScreen(0)
#define show()           swapBuffers(true)
#define Color(x,y,z)     Color444(x/16,y/16,z/16)
#define LED_BLACK           0
#define BLE_LOCAL_NAME "Smart Sticker"
// BLEのサービスUUID。適当なUUID(ランダムで生成したものがよい)を設定して下さい。
#define BLE_SERVICE_UUID "133fe8d4-5197-4675-9d76-d9bbf2450bb4"
// BLEのCharacteristic UUID。適当なUUID(ランダムで生成したものがよい)を設定して下さい。
#define BLE_CHARACTERISTIC_UUID "0fc10cb8-0518-40dd-b5c3-c4637815de40"

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;

  int16_t  hpx = 1;
  int16_t  hpy = 0;
  int16_t  hpw = 4;
  int16_t  hph = 16;
  uint16_t WHITE = 0xffffff;
  uint16_t BLACK = 0x000000;
  uint16_t red = 0xfff000;//限界
  uint16_t green = matrix.Color333(0, 7, 0);//満タン
  uint16_t yellow = 0xffff00;//半分*/
  uint16_t blue = 0x0000ff;
  uint16_t orange = 0xffa00;
  uint16_t rightgreen =matrix.Color333(3, 3, 0);//満タン
  int16_t tx = 0;
  int16_t ty = 0;



static const uint16_t PROGMEM
 ddriver[16][32]={
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0 ,0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0, 0, 0, 0, WHITE, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE,WHITE , 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
 };

 static const uint16_t PROGMEM
 bdriver[16][32]={
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue, blue, blue, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0 ,0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue, blue, blue, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue, blue, blue, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, 0, 0, 0, 0, 0, WHITE, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue, blue, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue,blue , 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, blue, blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
 };

static const uint16_t PROGMEM
 rdriver[16][32]={
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red, red, red, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0 ,0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red, red, red, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red, red, red, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, 0, 0, 0, 0, 0, WHITE, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, red, red, red, red, red, red, red, red, red, red, red, red, red, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, red, red, red, red, red, red, red, red, red, red, red, red, red, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red, red, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red,red , 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, red, red, red, red, red, red, red, red, red, red, red, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, red, red, red, red, red, red, red, red, red, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, red, red, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
 };

 static const uint16_t PROGMEM
 ydriver[16][32]={
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, yellow, yellow, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0 ,0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, yellow, yellow, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, yellow, yellow, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, 0, 0, 0, 0, 0, WHITE, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, yellow, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow,yellow , 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, yellow, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, yellow, yellow, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
 };

 static const uint16_t PROGMEM
 gdriver[16][32]={
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green, green, green, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0 ,0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green, green, green, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green, green, green, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, 0, 0, 0, 0, 0, WHITE, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, green, green, green, green, green, green, green, green, green, green, green, green, green, 0, 0, 0, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, 0, 0, green, green, green, green, green, green, green, green, green, green, green, green, green, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green, green, 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green,green , 0, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, 0, 0, 0, green, green, green, green, green, green, green, green, green, green, green, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, green, green, green, green, green, green, green, green, green, 0, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, green, green, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, 0, 0},
 };


static const uint16_t PROGMEM
  newmark[16][32]={
 	  {0,0,0,0,0,0,0,0,0,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,WHITE,0,0,0,0,0,0,0,0,WHITE,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,WHITE,0,0,0,0,0,0,WHITE,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,WHITE,0,0,0,0,WHITE,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,WHITE,0,0,WHITE,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,WHITE,WHITE,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,green,green,green,WHITE,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,green,green,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,green,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  };

  static const uint16_t PROGMEM
  newmark1[16][32]={
 	  {0,0,0,0,0,0,blue,0,0,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,blue,blue,blue,0,WHITE,yellow,WHITE,0,0,0,0,0,0,0,0,WHITE,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,blue,blue,blue,blue,blue,WHITE,yellow,yellow,WHITE,0,0,0,0,0,0,WHITE,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,blue,blue,blue,blue,blue,WHITE,yellow,yellow,yellow,WHITE,0,0,0,0,WHITE,green,green,green,WHITE,0,0,blue,0,0,0,0,0,0},
    {0,0,0,0,0,blue,blue,blue,0,WHITE,yellow,yellow,yellow,yellow,WHITE,0,0,WHITE,green,green,green,green,WHITE,0,blue,blue,blue,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,WHITE,WHITE,green,green,green,green,green,WHITE,blue,blue,blue,blue,blue,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,blue,blue,blue,blue,blue,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,blue,blue,blue,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,green,green,green,WHITE,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,green,green,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,green,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  };

  static const uint16_t PROGMEM
  newmark2[16][32]={
 	  {0,0,0,0,0,0,blue,0,0,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,blue,blue,blue,0,WHITE,yellow,WHITE,0,0,0,0,0,0,0,0,WHITE,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,blue,blue,blue,blue,blue,WHITE,yellow,yellow,WHITE,0,0,0,0,0,0,WHITE,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,blue,blue,blue,blue,blue,WHITE,yellow,yellow,yellow,WHITE,0,0,0,0,WHITE,green,green,green,WHITE,0,0,blue,0,0,0,0,0,0},
    {0,0,0,0,0,blue,blue,blue,0,WHITE,yellow,yellow,yellow,yellow,WHITE,0,0,WHITE,green,green,green,green,WHITE,0,blue,blue,blue,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,WHITE,WHITE,green,green,green,green,green,WHITE,blue,blue,blue,blue,blue,0,0,0,0},
    {0,0,blue,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,blue,blue,blue,blue,blue,0,0,0,0},
    {0,blue,blue,blue,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,blue,blue,blue,0,0,0,0,0},
    {blue,blue,blue,blue,blue,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {blue,blue,blue,blue,blue,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
    {0,blue,blue,blue,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,blue,0,0,WHITE,yellow,yellow,yellow,yellow,green,green,green,green,WHITE,0,0,0,0,blue,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,blue,blue,blue,0,0,WHITE,yellow,yellow,yellow,green,green,green,WHITE,0,0,0,0,blue,blue,blue,0,0,0,0,0},
    {0,0,0,0,0,0,blue,blue,blue,blue,blue,0,0,WHITE,yellow,yellow,green,green,WHITE,0,0,0,0,blue,blue,blue,blue,blue,0,0,0,0},
    {0,0,0,0,0,0,blue,blue,blue,blue,blue,0,0,0,WHITE,yellow,green,WHITE,0,0,0,0,0,blue,blue,blue,blue,blue,0,0,0,0},
    {0,0,0,0,0,0,0,blue,blue,blue,0,0,0,0,0,WHITE,WHITE,0,0,0,0,0,0,0,blue,blue,blue,0,0,0,0,0},
  };

  static const uint16_t PROGMEM
   oldermark1[16][32]{
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,WHITE,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,WHITE,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,WHITE,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,WHITE,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,WHITE,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,WHITE,orange,orange,WHITE,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,WHITE,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,WHITE,orange,orange,orange,orange,WHITE,rightgreen,rightgreen,rightgreen,rightgreen,rightgreen,WHITE,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,WHITE,orange,orange,orange,orange,WHITE,WHITE,rightgreen,rightgreen,rightgreen,WHITE,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,WHITE,orange,orange,orange,orange,orange,WHITE,WHITE,WHITE,green,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,WHITE,orange,orange,orange,orange,orange,orange,WHITE,WHITE,WHITE,green,green,green,green,green,WHITE,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,WHITE,orange,orange,orange,WHITE,yellow,yellow,yellow,WHITE,WHITE,green,green,green,green,WHITE,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,WHITE,yellow,yellow,yellow,yellow,yellow,WHITE,green,green,green,WHITE,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,WHITE,green,green,WHITE,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,WHITE,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,WHITE,yellow,yellow,yellow,yellow,yellow,yellow,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,yellow,yellow,yellow,yellow,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

   };

static const uint16_t PROGMEM
  babysleep[14][16]{
{0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0},
{0,0,0,0,WHITE,0,0,0,0,0,0,WHITE,0,0,0,0},
{0,0,0,WHITE,0,WHITE,0,0,0,0,0,0,WHITE,0,0,0},
{0,0,WHITE,0,0,WHITE,0,WHITE,0,0,0,0,0,WHITE,0,0},
{0,WHITE,0,0,0,0,WHITE,0,WHITE,0,0,0,0,0,WHITE,0},
{WHITE,0,0,0,0,0,0,WHITE,0,0,0,0,0,0,0,WHITE},
{WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE},
{0,WHITE,0,0,0,WHITE,WHITE,0,0,WHITE,WHITE,0,0,0,WHITE,0},
{0,0,WHITE,0,0,0,0,0,0,0,0,0,0,WHITE,0,0},
{0,WHITE,2,0,0,0,0,WHITE,WHITE,0,0,0,0,WHITE,WHITE,0},
{0,0,WHITE,WHITE,WHITE,0,0,0,0,0,0,WHITE,WHITE,WHITE,0,0},
{0,0,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,0,0},
{0,0,0,WHITE,WHITE,0,0,0,0,0,0,WHITE,WHITE,0,0,0},
{0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0},

};

static const uint16_t PROGMEM
  newmark_v1[16][14]={
 	  {WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE},
    {WHITE,yellow,WHITE,0,0,0,0,0,0,0,0,WHITE,green,WHITE},
    {WHITE,yellow,yellow,WHITE,0,0,0,0,0,0,WHITE,green,green,WHITE},
    {WHITE,yellow,yellow,yellow,WHITE,0,0,0,0,WHITE,green,green,green,WHITE},
    {WHITE,yellow,yellow,yellow,yellow,WHITE,0,0,WHITE,green,green,green,green,WHITE},
    {WHITE,yellow,yellow,yellow,yellow,yellow,WHITE,WHITE,green,green,green,green,green,WHITE},
    {WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE},
    {WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE},
    {WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE},
    {WHITE,yellow,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,green,WHITE},
    {0,WHITE,yellow,yellow,yellow,yellow,yellow,green,green,green,green,green,WHITE,0},
    {0,0,WHITE,yellow,yellow,yellow,yellow,green,green,green,green,WHITE,0,0},
    {0,0,0,WHITE,yellow,yellow,yellow,green,green,green,WHITE,0,0,0},
    {0,0,0,0,WHITE,yellow,yellow,green,green,WHITE,0,0,0,0},
    {0,0,0,0,0,WHITE,yellow,green,WHITE,0,0,0,0,0},
    {0,0,0,0,0,0,WHITE,WHITE,0,0,0,0,0,0},
  };


static const uint16_t PROGMEM
  heartmark[6][9]{
    {0,0,red,0,0,0,0,red,0},
    {0,red,red,red,0,0,red,red,red},
    {0,red,red,red,red,red,red,red,red,},
    {0,0,red,red,red,red,red,red,0,},
    {0,0,0,red,red,red,red,0,0},
    {0,0,0,0,red,red,0,0,0},
  };


void numberText(char number1,char number2,int16_t tx,int16_t ty,int16_t txx, int16_t tyy){
   matrix.drawChar(tx, ty, number1, WHITE, BLACK,2);
   matrix.drawChar(txx, tyy, number2, WHITE, BLACK,2);

};

void drawHP(uint16_t color,int16_t ipx, int16_t ipy,int16_t ipw,int16_t iph){

   matrix.drawRect(hpx,hpy,hpw,hph,color);
   matrix.drawRect(ipx,ipy,ipw,iph,color);
   delay(10000);
};

void rdrawHP(uint16_t color,int16_t ipx, int16_t ipy,int16_t ipw,int16_t iph){
   matrix.drawRect(hpx,hpy,hpw,hph,color);
   for (ipy = 15; ipy > 13; ipy--){
   
   matrix.drawRect(ipx,ipy,ipw,ipy,color);
   delay(10);
   }
   
   
   delay(10000);
};



void heart_newmark(){
  matrix.drawRGBBitmap(9, 0, newmark_v1[0],14,16);//初心者マーク
  matrix.drawRGBBitmap(0, 0, heartmark[0],9, 6);
  matrix.drawRGBBitmap(0, 9, heartmark[0],9, 6);
  matrix.drawRGBBitmap(23, 0, heartmark[0],9, 6);
  matrix.drawRGBBitmap(23, 10, heartmark[0],9, 6);


}

void changenewmark(){
      matrix.drawRGBBitmap(0, 0, newmark[0],mw, mh );//画像表示
      delay(1500);
      matrix.drawRGBBitmap(0, 0, newmark1[0],mw, mh );//画像表示
      delay(1500);
      matrix.drawRGBBitmap(0, 0, newmark2[0],mw, mh );//画像表示
      delay(1500);
};


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("connect");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("disconnect");
      deviceConnected = false;
    };
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    
    Serial.println("read");
    pCharacteristic->setValue("スマートステッカーと接続しました");
  };


  void onWrite(BLECharacteristic *pCharacteristic) {
    
    Serial.println("write");
    std::string value = pCharacteristic->getValue();
    int num = std::atoi(value.c_str());
   /* M5.Lcd.println(value.c_str());
    M5.Lcd.println(num);
    matrix->fillScreen(LED_BLACK);
    matrix->setCursor(0,0);//表示する位置

    matrix->print(value.c_str());//表示する文字*/
    /*if(--x < -45) {
      x = matrix->width();
    //if(++pass >= 3) pass = 0;//色を変える処理
      matrix->setTextColor(colors[0]); //文字の色を繰り返し変える処理（三色
    }*/
    
    delay(60);
    //Serial.println(value.c_str());
    if(num == 1){
    /*matrix.drawRGBBitmap(0, 0, gdriver[0],mw, mh );//画像表示
       drawHP(matrix.Color333(0, 7, 0),2,13,2,16);//満タン
      delay(1000);
       matrix.clear(); 
      matrix.drawRGBBitmap(0, 0, ydriver[0],mw, mh );//画像表示
      drawHP(orange,2,7,2,13);//半分
      delay(1000);
       matrix.clear(); 
      matrix.drawRGBBitmap(0, 0, rdriver[0],mw, mh );//画像表示
      drawHP(red,2,0,2,14);//限界
       //drawHP(matrix.Color333(0, 7, 0),2,1,2,14);//満タン
      delay(1000);
      matrix.clear(); */

    }

    else if(num == 2){
     matrix.clear(); 
     matrix.drawRGBBitmap(0, 0, newmark[0],mw, mh );//画像表示
     //drawHP(yellow,2,8,2,7);//半分
     //matrix.show();
    }
    
    else if(num == 3){
      matrix.clear();
      matrix.drawRGBBitmap(0, 0, oldermark1[0],mw, mh );//画像表示
      //drawHP(red,2,13,2,3);//限界
      //matrix.show();
    }
    else if(num == 4){
     matrix.clear();
     numberText('7','7',5,1,17,1);
    }
    else if(num == 5){
      matrix.clear(); 
     numberText('5','5',5,1,17,1);
    }
    
    else if(num == 6){
       matrix.clear();
      numberText('1','7',5,1,17,1);
    }
    else if(num == 7){
       matrix.clear();
     numberText('8','1',6,1,18,1);
    }
    else if(num == 8){
       matrix.clear();
     numberText('6','9',6,1,18,1);
    }
    else if(num == 9){
       matrix.clear();
     numberText('2','9',6,1,18,1);
    }
    else if(num == 10){
       matrix.clear();
     numberText('4','3',5,1,17,1);
    }
    else if(num == 11){
       matrix.clear();
     numberText('8','7',6,1,18,1);
    }
    else if(num == 12){
       matrix.clear();
     numberText('9','7',6,1,18,1);
    }
     else if(num == 13){
      matrix.drawRGBBitmap(0, 0, gdriver[0],mw, mh );//画像表示
       drawHP(matrix.Color333(0, 7, 0),2,0,2,16);//満タン
       matrix.clear(); 
    }
    else if(num == 14){
      matrix.drawRGBBitmap(0, 0, ydriver[0],mw, mh );//画像表示
      drawHP(orange,2,7,2,13);//半分
       matrix.clear(); 
    }
    else if(num == 15){
       matrix.drawRGBBitmap(0, 0, rdriver[0],mw, mh );//画像表示
      drawHP(red,2,13,2,15);//限界
      matrix.clear(); 
    }

  
  }
};



/*void numberText(uint8_t number){
  matrix.setTextSize(20,20);
  matrix.setCursor(tx = 10, ty = 10);
  matrix.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
  matrix.
};*/



void initBLEServise() {
  BLEDevice::init(BLE_LOCAL_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(BLE_SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         BLE_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                        
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();  

}

int ipy=0;
void setup() {
  matrix.begin();
  matrix.clear();
  initBLEServise();
  //matrix.setBrightness(BRIGHT);

 
}


void loop() {
     
 changenewmark();
 //heart_newmark();
      
}
