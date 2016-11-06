

void setup() {
  // put your setup code here, to run once:
  Serial0.begin(115200);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial0.available() > 0)
  {
    int r = Serial0.read();
    Serial.write(r);
    delay(1);
  }
}