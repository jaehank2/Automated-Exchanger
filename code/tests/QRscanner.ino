#include <SoftwareSerial.h>

SoftwareSerial qr(0, 1);  // Rx, Tx
const unsigned long timeout = 10000; // timeout value for QR set to 2 seconds
String qr1 = "3213";
String qr2 = "3214";
String qr3 = "3212";
bool check = false;

int datapin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  qr.begin(115200);
  Serial.println("Initializing QR");
  pinMode(datapin, OUTPUT);
  digitalWrite(datapin, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
//  unsigned long startTime = millis(); // timer starts
//  while (millis() - startTime < timeout){
//  if (qr.available()){
//    String data = "";
//    Serial.println("Reading QR");
//    while (qr.available()){
//      int input = qr.read();
//      data += input;
//      Serial.print(input);
//      delay(5);
//    }
//    Serial.println();
//    if (data == qr1) {
//      check = true;
//      Serial.println("Sucess");
//    }
//  }
//  }
  check = QRcheck();
  if (check == false){
//    Serial.println("check false");
    digitalWrite(datapin, LOW);
  }
  else if (check == true){
//    Serial.println("check true");
    check = false;
    digitalWrite(datapin, HIGH);
  }
//  check = QRcheck();
//  if (check == false) {
//    Serial.println("FAILED");
//  }
//  else if (check == true) {
//    Serial.println("SUCESS");
//  }

}

bool QRcheck() {
  unsigned long startTime = millis(); // timer starts

  while (millis() - startTime < timeout){
    if (qr.available()){
      String val = "";
      while (qr.available()){
        int input = qr.read();
        val += input;
        delay(5);
      }
      if (val == qr1 || val == qr2 || val == qr3) {
        return true;
        break;
      }
      Serial.println("not equal");
      Serial.print("val :");
      Serial.println(val);
      break;
    }
  }

  return false;
}
