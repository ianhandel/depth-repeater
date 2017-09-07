#include <TinyGPS++.h>

TinyGPSPlus gps;

TinyGPSCustom depthBT(gps, "SDDPT", 1);
TinyGPSCustom depthOFFSET(gps, "SDDPT", 2);

#define  OK_WAIT 3600

int I2C_SCL = 12;                 
int I2C_SDA = 3;                 

static char cl_data[12] = {0x7c,0xce,0x80,0xe0,0xf8,0x70,0x00,0x00,0x00,0x00,0x00,0x00};
static char no_data[12] = {0x7c,0xce,0x80,0xe0,0xf8,0x70,0x10,0x04,0x00,0x00,0x01,0x00};
static float depthBelowKeel = 0;
static long lastDepth = millis() - OK_WAIT;

void setup()
{
  pinMode(I2C_SCL, OUTPUT);      // sets the digital pin as output
  pinMode(I2C_SDA, OUTPUT);      // sets the digital pin as output
  digitalWrite(I2C_SCL, HIGH);   // sets the pin on
  digitalWrite(I2C_SDA, HIGH);   // sets the pin on
  Serial.begin(4800);
}


void I2C_start()
{
  digitalWrite(I2C_SCL, HIGH);   // sets the pin on
  delayMicroseconds(1);
  digitalWrite(I2C_SDA, LOW);   // sets the pin on
  delayMicroseconds(2);        // pauses for 50 microseconds 
  digitalWrite(I2C_SCL, LOW);   // sets the pin on
  delayMicroseconds(1);
  digitalWrite(I2C_SCL, HIGH);   // sets the pin on
}

void I2C_stop()
{
  digitalWrite(I2C_SCL, HIGH);   // sets the pin on
  delayMicroseconds(3);        // pauses for 50 microseconds 
  digitalWrite(I2C_SDA, HIGH);   // sets the pin on
  delayMicroseconds(3);        // pauses for 50 microseconds 
}

void I2C_writebyte(byte data)
{
  int i=0;
  bool sending;
  // write byte
  for(i=7;i>=0;i--)
  {
     sending = (data >> i) & 0x01;
     digitalWrite(I2C_SDA, sending);
     delayMicroseconds(1);
     digitalWrite(I2C_SCL, HIGH);
     delayMicroseconds(1);
     digitalWrite(I2C_SCL, LOW);
  }
  // write ack
  delayMicroseconds(1);
  digitalWrite(I2C_SDA, HIGH);
  delayMicroseconds(2);
  digitalWrite(I2C_SCL, HIGH);
  delayMicroseconds(2);
  digitalWrite(I2C_SCL, LOW);
  delayMicroseconds(2);
}

void I2C_talk_to_clipper(char *data)
{
  char *ptr;
  ptr = data;
  I2C_start();
  I2C_writebyte(*ptr++); 
  I2C_writebyte(*ptr++); // =   
  I2C_writebyte(*ptr++); // =
  I2C_writebyte(*ptr++); // =
  I2C_writebyte(*ptr++); // =
  I2C_writebyte(*ptr++); // =
  delayMicroseconds(325);
  I2C_writebyte(*ptr++);
  delayMicroseconds(325);
  I2C_writebyte(*ptr++);
  delayMicroseconds(325);
  I2C_writebyte(*ptr++); // =
  delayMicroseconds(325);
  I2C_writebyte(*ptr++); // =
  delayMicroseconds(325);
  I2C_writebyte(*ptr++); // =
  delayMicroseconds(325);
  I2C_writebyte(*ptr++);
  delayMicroseconds(20);
  I2C_stop();
}



char digit3[10][6] = {                   // from https://en.wikipedia.org/wiki/Seven-segment_display
                   { 0, 0xbb,0,0,0,0  }, // zero, a,b,c,d,e,f,/g
                   { 0, 0x11, 0,0,0,0 }, // one /a,b,c,/d,/e,/f,/g
                   { 0, 0x9e, 0,0,0,0 }, // two a,b,/c,d,e,/f,g
                   { 0, 0x97, 0,0,0,0 }, // three a,b,c,d,/e,/f,g
                   { 0, 0x35, 0,0,0,0 }, // four /a,b,c,/d,/e,f,g
                   { 0, 0xa7, 0,0,0,0 }, // five a,/b,c,d,/e,f,g
                   { 0, 0xaf, 0,0,0,0 }, // six a,/b,c,d,e,f,g
                   { 0, 0x91, 0,0,0,0 }, // seven a,b,c,/d,/e,/f,/g
                   { 0, 0xbf, 0,0,0,0 }, // eight a,b,c,d,e,f,g
                   { 0, 0xb7, 0,0,0,0 }, // nine a,b,c,d,/e,f,g
                 };
                 
char digit2[10][6] = {                   // from https://en.wikipedia.org/wiki/Seven-segment_display
                   { 0xee, 0, 0,0,0,0  },// zero, a,b,c,d,e,f,/g
                   { 0x44, 0, 0,0,0,0 }, // one /a,b,c,/d,/e,/f,/g
                   { 0xb6, 0, 0,0,0,0 }, // two a,b,/c,d,e,/f,g
                   { 0xd6, 0, 0,0,0,0 }, // three a,b,c,d,/e,/f,g
                   { 0x5c, 0, 0,0,0,0 }, // four /a,b,c,/d,/e,f,g
                   { 0xda, 0, 0,0,0,0 }, // five a,/b,c,d,/e,f,g
                   { 0xfa, 0, 0,0,0,0 }, // six a,/b,c,d,e,f,g
                   { 0x46, 0, 0,0,0,0 }, // seven a,b,c,/d,/e,/f,/g
                   { 0xfe, 0, 0,0,0,0 }, // eight a,b,c,d,e,f,g
                   { 0xde, 0, 0,0,0,0 }, // nine a,b,c,d,/e,f,g
                 };
                 
char digit1[10][6] = {                      // from https://en.wikipedia.org/wiki/Seven-segment_display
                   { 0, 0, 0,0,0x2e,0xc0 }, // zero, a,b,c,d,e,f,/g
                   { 0, 0, 0,0,0x04,0x40 }, // one /a,b,c,/d,/e,/f,/g
                   { 0, 0, 0,0,0x27,0x80 }, // two a,b,/c,d,e,/f,g
                   { 0, 0, 0,0,0x25,0xC0 }, // three a,b,c,d,/e,/f,g
                   { 0, 0, 0,0,0x0d,0x40 }, // four /a,b,c,/d,/e,f,g
                   { 0, 0, 0,0,0x29,0xC0 }, // five a,/b,c,d,/e,f,g
                   { 0, 0, 0,0,0x2b,0xC0 }, // six a,/b,c,d,e,f,g
                   { 0, 0, 0,0,0x24,0x40 }, // seven a,b,c,/d,/e,/f,/g
                   { 0, 0, 0,0,0x2f,0xc0 }, // eight a,b,c,d,e,f,g
                   { 0, 0, 0,0,0x2d,0xc0 }, // nine a,b,c,d,/e,f,g
                 };

void loop()
{
  static byte num[3] = {0, 0, 0};
  static byte decimal = 1;
  
    cl_data[6] = digit3[num[0]][0]  | digit2[num[1]][0] | digit1[num[2]][0];
    cl_data[7] = digit3[num[0]][1]  | digit2[num[1]][1] | digit1[num[2]][1];
    cl_data[8] = digit3[num[0]][2]  | digit2[num[1]][2] | digit1[num[2]][2];
    cl_data[9] = digit3[num[0]][3]  | digit2[num[1]][3] | digit1[num[2]][3];
    cl_data[10] = digit3[num[0]][4] | digit2[num[1]][4] | digit1[num[2]][4];
    cl_data[11] = digit3[num[0]][5] | digit2[num[1]][5] | digit1[num[2]][5];

    // depth
    cl_data[6] = cl_data[6] | 0x01;
    // dp
    if(decimal == 1){
      cl_data[9] = cl_data[9] | 0x80;
    }

    num[0] = (num[0] + 1) % 10;
    num[1] = (num[1] + 1) % 10;
    num[2] = (num[2] + 1) % 10;




     /*
     char nmea[] = "$SDDPT,10.1,-1.5,*62\r\n";

     for(int ii = 0; ii<22; ii++){
           gps.encode(nmea[ii]);
     }
     */
       while (Serial.available() > 0)
    gps.encode(Serial.read());

    if(depthBT.isUpdated() & depthOFFSET.isUpdated()){
      lastDepth = millis(); 
    }

    if(millis() - lastDepth > OK_WAIT){
      depthBelowKeel = 0;
      I2C_talk_to_clipper(no_data);
      delay(250);
      I2C_talk_to_clipper(no_data);
      delay(250);
    }else{
     depthBelowKeel = atof(depthBT.value()) + atof(depthOFFSET.value());
     I2C_talk_to_clipper(cl_data);
     delay(250);
     I2C_talk_to_clipper(cl_data);
     delay(250);
    }

     Serial.println(depthBelowKeel);
     Serial.println(millis() - lastDepth);
     Serial.println("");
}     



//$SDDPT,10.1,-1.5,*62
