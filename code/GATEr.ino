#include <hidboot.h> // card reader
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "HX711.h"
#include <Servo.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
// #include <LiquidCrystal.h>

// loadcell
#define calibration_factor -7050.0
#define DOUT 4
#define CLK 3

HX711 scale;

Servo servoD; // continuous servo for dispensing
Servo servoR; // non-continuous servo for retrieving

SoftwareSerial qr(0, 1);  // Rx, Tx

// LCD Display
//int contrast = 60;
//const int v0 = A2, rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// QR codes
String qr1 = "3213";
String qr2 = "3214";
String qr3 = "3212";

const int buttonL = 8;
const int buttonR = 7;

const int servoD_pin = 2;
const int servoR_pin = 6;

const unsigned long timeout = 10000; // timeout value for QR set to 10 seconds

// Select bits for mux
int a0 = A0;
int a1 = A1;
int a2 = A2;

int pos = 0;
int num_tokens = 2;
bool card_valid = false;
bool start = false;

// card reader
String s = "";
bool card_read = false;
bool company = false;
bool unused = false;
bool uin = false;

// String John = "661111111";

struct student {
  String netID;
  int addr;
  int tokenNum;
};

student jaehank2;
student henryg3;
student* studentList[2] = {&jaehank2, &henryg3};
int idx = 0;

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  return;
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  return;

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  return;
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  s += ((char)key);
  if (card_read == true && s.length() == 17){
    Serial.println();
    s = "";
    card_read = false;
    company = false;
    unused = false;
    uin = false;
    return;
  }
  else if (company == false && s.length() == 19){
    // Serial.println(s);
    s = "";
    company = true;
  }
  else if (company == true && s.length() == 21){
    Serial.println(s);
    s = "";
    unused = true;
  }
  else if (s.length() == 14 && unused == true && card_read == false){
    s = "";
    uin = true;
  }
  else if (s.length() == 9 && uin == true && card_read == false){
    Serial.print("UIN: ");
    Serial.println(s);
    if (s == jaehank2.netID){
      idx = 0;
      card_valid = true;
    }
    else if (s == henryg3.netID){
      idx = 1;
      card_valid = true;
    }
    else {
      card_valid = false;
    }
    card_read = true;
    s = "";
  }
};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
  qr.begin(115200);
  Serial.println("Start");
  if (Usb.Init() == -1){
    Serial.println("OSC did not start.");
  }
  delay( 200 );
  HidKeyboard.SetReportParser(0, &Prs);
  // initialize student info
  jaehank2.netID = "661111111";
  jaehank2.addr = 0;
  jaehank2.tokenNum = EEPROM.read(jaehank2.addr);

  henryg3.netID = "651111111";
  henryg3.addr = 1;
  henryg3.tokenNum = EEPROM.read(henryg3.addr);
    
  // initialize push buttons
  pinMode(buttonL, INPUT);
  pinMode(buttonR, INPUT);
  // have all LEDs on
  setLED(255, 255, 255);
  // initialize non-continuous servo motor
  servoR.write(0);
  servoR.attach(servoR_pin);
  servoR.write(0);
  // initialize scale
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare(); // Assuming there is no weight on the scale at start up, reset the scale to 0

}

void loop() {
  // put your main code here, to run repeatedly:

  // save initial button status
  int b1 = digitalRead(buttonL);
  int b2 = digitalRead(buttonR);

//  Serial.println(b1);
//  Serial.println(b2);
  Usb.Task();

  start = true;

  if (card_valid == true){
    Serial.println("card successfully read");
  }

  // TODO: set LED status for IDLE/init - Y0
  setLED(0, 0, 0);
  
  while (card_valid == true){
    // TODO: set LED status for SELECT DEPOSIT - Y1
    setLED(255, 0, 0);
    
    if (start == true){
      start = false;
      Serial.println();
      Serial.println();
      studentList[idx]->tokenNum = EEPROM.read(studentList[idx]->addr);
      
      Serial.print("your current token num: ");
      Serial.println(studentList[idx]->tokenNum);
      Serial.println("Select Deposit option");
    }
    
    // get number of tokens for the user
    // check pushbutton for using token
    if (b1 != digitalRead(buttonL)){
      Serial.println("Chose token");
      card_valid = false;
      // if not enough tokens, exit
      if (studentList[idx]->tokenNum <= 0){
        Serial.println("invalud num of tokens");
        // TODO: set LED status for invalid tokens - Y5
        setLED(255, 0, 255);
        delay(2000);
        
        break;
      }
      // else update token and dispense
      setLED(0, 0, 255);  // updated - Y4
      delay(2000);
      studentList[idx]->tokenNum -= 1;
      EEPROM.write(studentList[idx]->addr, studentList[idx]->tokenNum);

      Serial.print("your updated token num: ");
      Serial.println(studentList[idx]->tokenNum);
      // TODO: set LED status for dispense - Y3
      setLED(255, 255, 0);
      
      dispense();
      break;
    }

    // check pushbutton for returning old container
    if (b2 != digitalRead(buttonR)){
      setLED(255, 255, 255);  // Y7
      Serial.println("Chose container: put container and scan");
      card_valid = false;
      bool qrValid = QRcheck();
      Serial.println(qrValid);
      float weight = scale.get_units(5);
      Serial.print("weight: ");
      Serial.println(weight);
      
      if (weight <= 5.0 && qrValid == true){
        // TODO: set LED status for retrieving - Y2
        setLED(0, 255, 0);
        
        retrieve();
//        b1 = digitalRead(buttonL);
//        b2 = digitalRead(buttonR);
        // TODO: set LED status for SELECT DISPENSE - Y1
        setLED(255, 0, 0);
        
        Serial.println("SELECT dispense option");
        
        // spin until button pressed
        while (b1 == digitalRead(buttonL) && b2 != digitalRead(buttonR));
        
        // if user pressed token for dispense
        if (b1 != digitalRead(buttonL)){
          studentList[idx]->tokenNum += 1;
          EEPROM.write(studentList[idx]->addr, studentList[idx]->tokenNum);
          Serial.print("your updated token num: ");
          Serial.println(studentList[idx]->tokenNum);
          // TODO: set LED status for token updated - Y4
          setLED(0, 0, 255);
          delay(2000);

          break;
        }
        // else user pressed new container for dispense
        // TODO: set LED status for dispense - Y3
        setLED(255, 255, 0);

        dispense();
        break;
        
      }
      // invalid container
      else {
        Serial.println("invalid container");
        // TODO: set LED status for invalid container - Y5
        setLED(255, 0, 255);
        delay(2000);
        if (qr.available()){
          while (qr.available()){
            int t = qr.read();
            delay(5);
          }
        }

        break;
      }
    }
  }

}

void dispense() {
  Serial.println("DISPENSING");
//   return;
  servoD.attach(servoD_pin);
  servoD.write(0);  // rotate CW at full speed
  delay(2500);
  servoD.detach();
}

void retrieve() {
  Serial.println("RETRIEVING");
//   return;
  for (pos = 0; pos <=110; pos++){
    servoR.write(pos);
    delay(10);
  }
  for (pos = 110; pos >= 0; pos--){
    servoR.write(pos);
    delay(10);
  }
}

bool QRcheck() {
  unsigned long startTime = millis(); // timer starts
  String val = "";
  while (millis() - startTime < timeout){
    if (qr.available()){
      Serial.println("reading QR");
//      String val = "";
      while (qr.available()){
        int input = qr.read();
        val += input;
        delay(5);
      }
      Serial.println(val);
      if (val.length() > 4 || val == qr1 || val == qr2 || val == qr3) {
        Serial.println("QR successfully read");
        return true;
      }
      break;
    }
  }
  Serial.println("timed out");
  return false;
}

void setLED(int x, int y, int z){
  analogWrite(a0, x);
  analogWrite(a1, y);
  analogWrite(a2, z);
}
