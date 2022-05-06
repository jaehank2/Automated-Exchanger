int a0 = A0;
int a1 = A1;
int a2 = A2;

void setup() {
  // put your setup code here, to run once:
//  pinMode(a0, OUTPUT);
//  pinMode(a1, OUTPUT);
//  pinMode(a2, OUTPUT);
//  digitalWrite(a0, LOW);
//  digitalWrite(a1, LOW);
//  digitalWrite(a2, LOW);
   analogWrite(a0, 0);
   analogWrite(a1, 0);
   analogWrite(a2, 0);
   delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(a0, LOW);
//  digitalWrite(a1, LOW);
//  digitalWrite(a2, LOW);
//
//  delay(2000);

//  digitalWrite(a0, HIGH);
//  digitalWrite(a1, HIGH);
//  digitalWrite(a2, LOW);

//  delay(2000);

  analogWrite(a0, 255);
  analogWrite(a1, 255);
  analogWrite(a2, 0);
  delay(2000);
}
