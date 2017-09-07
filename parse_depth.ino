

static float depth;
static bool decimal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(4800);

}

void loop() {
  // put your main code here, to run repeatedly:


depth = Serial.parseFloat();

if(depth < 0) depth = 0;

if((int)depth/100%10 == 0){
  depth = depth * 10;
  decimal = true;
}else{
  decimal = false;
}
 
if(depth !=0){
    Serial.print(" ");
    if((int)depth/100%10 > 0) Serial.print((int)depth/100%10);
    Serial.print(" ");
    if((int)depth/10%10 > 0 | (int)depth/100%10 > 0 | decimal) Serial.print((int)depth/10%10);
    if(decimal){
     Serial.print(".");
    }else{
      Serial.print(" ");
    }
    Serial.println((int)depth/1%10);
    delay(100);
}


}
