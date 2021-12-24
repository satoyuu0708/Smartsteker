// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For WHITE6x32 RGB LED matrix:
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
/*#define OE   23
#define CLK  2
#define LAT  5
#define A   18
#define B   19
#define C   21
#define D   22
#define PIN_R1  16
#define PIN_G1  26
#define PIN_B1  17
#define PIN_R2  3
#define PIN_G2 25
#define PIN_B2 1*/
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
#define setBrightness(x) fillScreen(0) // no-op, no brightness on this board
#define clear()          fillScreen(0)
#define show()           swapBuffers(true)
#define Color(x,y,z)     Color444(x/16,y/16,z/16)
#define LED_BLACK           0
#define BLE_LOCAL_NAME "ESP32"
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
  int16_t red = 0xfff000;//限界
  int16_t green = matrix.Color333(0, 7, 0);//満タン
  int16_t yellow = 0xffff00;//半分*/

/*static const uint16_t PROGMEM
 driverrr1[16][32]={
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,WHITE,0,0,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,WHITE,0,0,WHITE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,WHITE,WHITE,WHITE,0,0,0,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0,0},
  {0,0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0},
  {0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0},
  {0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0},
  {0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0},
  {0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0},
  {0,0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,WHITE,WHITE,0,0,0,0,0},
  {0,0,0,0,0,0,0,WHITE,WHITE,WHITE,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,WHITE,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,0,0,WHITE,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,0,0,WHITE,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,WHITE,WHITE,WHITE,0,0,0,0,0},
      
};*/

static const uint16_t PROGMEM
 driverrr2[16][32]={
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

void drawHP(uint16_t color,int16_t ipx, int16_t ipy,int16_t ipw,int16_t iph){
   matrix.drawRect(hpx,hpy,hpw,hph,color);
   matrix.drawRect(ipx,ipy,ipw,iph,color);
   delay(10000);
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
     matrix.clear(); 
     matrix.drawRGBBitmap(0, 0, driverrr2[0],mw, mh );//画像表示
     drawHP(matrix.Color333(0, 7, 0),2,1,2,14);//半分
     matrix.show();

    }

    else if(num == 2){
     matrix.clear(); 
     matrix.drawRGBBitmap(0, 0, driverrr2[0],mw, mh );//画像表示
     drawHP(yellow,2,8,2,7);//半分
     matrix.show();
    }
    
    else if(num == 3){
      matrix.clear();
      matrix.drawRGBBitmap(0, 0, driverrr2[0],mw, mh );//画像表示
      drawHP(red,2,13,2,3);//限界
      matrix.show();
    }
  
  }
};


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


void setup() {
  matrix.begin();
  matrix.clear();
  initBLEServise();
  //matrix.setBrightness(BRIGHTNESS);
 
}


void loop() {
     
       //matrix.drawRGBBitmap(0, 0, driverrr2[0],mw, mh );//画像表示
       //drawHP(matrix.Color333(0, 7, 0),2,1,2,14);//半分
       //drawHP(yellow,2,8,2,7);//半分
       //drawHP(red,2,13,2,3);//限界
     
  
  
}
