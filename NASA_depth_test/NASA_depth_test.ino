
void setup() {
  Serial.begin(4800);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
}

void loop() {
  
  Serial.println("$SDDPT,10.1,-1.5,*62");
  
  delay(1000);
}

/*
$SDDPT,10.1,-1.5,*62
$SDDPT,100.7,-1.7,*56
$SDDPT,0.7,-1.5,*55
$SDDPT,11.7,-1.5,*65
$SDDPT,78,-1.5,*73
$SDDPT,198.7,-1.5,*55
$SDDPT,1.5,-1.5,*56
$SDDPT,999,-1.7,*47
$SDDPT,94,-1.7,*73
*/

