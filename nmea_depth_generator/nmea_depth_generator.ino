
// stuff for NMEA
const byte buff_len = 90;
char CRCbuffer[buff_len];

// create pre-dfined strings to control flexible output formatting
String cmd = "$SDDPT";
String sp = " ";
String delim = ",";
String splat = "*";
String reference = "R";
String units = "N";
String valid = "A";
String msg = "";
float DBT = 0;
float delta = 1;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(4800);
}

// the loop function runs over and over again forever
void loop() {
  //Serial.println("$SDDPT,10.1,-1.5,*62");

  char strAngle[8];
  char strSpeed[8];
  float offset = 0;
  msg = cmd + delim + DBT + delim + offset + delim + valid + splat;
  outputMsg(msg);
  delay(1000);
  DBT = DBT + delta;
  if(DBT >= 20){
    delta = 10;
  }
  if(DBT >= 150){
    delta = -10;
  }
  if(DBT <= 20 & delta == -10){
    delta = -1;
  }
if(DBT <= 0){
    delta = 1;
  }
}


// -----------------------------------------------------------------------
void outputMsg(String msg) {
  msg.toCharArray(CRCbuffer, sizeof(CRCbuffer)); // put complete string into CRCbuffer
  int crc = convertToCRC(CRCbuffer);

  Serial.print(msg);
  if (crc < 16) Serial.print("0"); // add leading 0
  Serial.println(crc, HEX);
}

// -----------------------------------------------------------------------
byte convertToCRC(char *buff) {
  // NMEA CRC: XOR each byte with previous for all chars between '$' and '*'
  char c;
  byte i;
  byte start_with = 0;
  byte end_with = 0;
  byte CRC = 0;

  for (i = 0; i < buff_len; i++) {
    c = buff[i];
    if (c == '$') {
      start_with = i;
    }
    if (c == '*') {
      end_with = i;
    }
  }
  
  for (i = start_with + 1; i < end_with; i++) { // XOR every character between '$' and '*'
      CRC = CRC ^ buff[i] ;  // compute CRC
    }

  return CRC;
  //based on code by Elimeléc López - July-19th-2013
}


