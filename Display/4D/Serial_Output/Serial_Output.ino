void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.write(0x00);
//  Serial.write(0xBC);
//  Serial.write(0x34);
  for(int i = 0; i<13500;i+=100) {
    Serial.write(0x00);
    //Serial.write(i);
    Serial.write(lowByte(i));
    Serial.write(highByte(i));

  delay(50);
  }
}
