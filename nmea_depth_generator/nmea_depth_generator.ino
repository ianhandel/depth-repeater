

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(4800);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("$SDDPT,10.1,-1.5,*62");
  delay(1000);
  Serial.println("$SDDPT,11.7,-1.5,*65");
  delay(1000);
  Serial.println("$$SDDPT,0.7,-1.5,*55");
  delay(1000);
  Serial.println("$SDDPT,78,-1.5,*73");
  delay(1000);
  Serial.println("$SDDPT,999,-1.7,*47");
  delay(1000);
  Serial.println("$SDDPT,94,-1.7,*73");
  delay(1000);
  Serial.println("$SDDPT,100.7,-1.7,*56");
  delay(1000);
  Serial.println("$SDDPT,1.5,-1.5,*56");
  delay(2000);
}


