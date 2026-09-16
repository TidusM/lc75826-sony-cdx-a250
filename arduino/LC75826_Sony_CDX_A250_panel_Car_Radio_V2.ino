/**********************************************************************/
/*
This code is not clean and far from perfect, that's just
a reference to extract ideas and adapt to your solution.
you can replace the BIN values with HEX... I leave it in BIN
because it is easier to relate the segment number with
the position of the bit in BIN.
Of course, a library can be created for this purpose! But I won't 
take the time to do that, I'll leave it up to you!
*/
/**********************************************************************/
/* This code show how is possible work with the driver LC75826 Sony */
/* 
/* Also pinout is equivalent to the PT6526, check the datasheet */

/*
* Note: DD is the direction data. 
• CCB address ....... "41H" 
• D1 to D208 ......... Display data 
• P0 to P3 .............. Segment output port/general-purpose output port switching control data 
• DR ...................... 1/2-bias drive or 1/3-bias drive switching control data 
• DN ...................... 200-/208-segment display switching control data 
• FC0 to FC2 ......... Common/segment output waveform frame frequency control data 
• OC ...................... Internal oscillator operating mode/external clock operating mode switching control data 
• SC ...................... Segments on/off control data 
• BU ...................... Normal mode/power-saving mode control data 
*
* P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction!
*/
/*Begining of Auto generated code by Atmel studio */
#define VFD_in 8  // This is the pin number 8 on Arduino UNO  // You can use a resistor of 1Kohms to protect this line!
#define VFD_clk 9 // This is the pin number 9 on Arduino UNO  // You can use a resistor of 1Kohms to protect this line!
#define VFD_ce 10 // This is the pin number 10 on Arduino UNO // You can use a resistor of 1Kohms to protect this line!
#define addr 0x41 // This is the address of our IC

unsigned int shiftBit=0;
unsigned int nBitOnBlock=0; // Used to count number of bits and split to 8 bits... (number of byte)
unsigned int nByteOnBlock=0; 
unsigned int sequencyByte=0x00;
byte Aa, Ab, Ac, Ad, Ae, Af, Ag, Ah;
byte blockBit =0x00;
uint8_t nMaskBit = 0x00; //This variable is used only to print the bit value of "netMask"

#define BUTTON_PIN 2 //Att check wich pins accept interrupts... Uno is 2 & 3, This is our trigger button
volatile byte buttonReleased = false;

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

uint8_t LEDcolor = 0x00;
uint8_t LEDinfo = 0x00;

bool forward = false;
bool backward = false;
bool isRequest = true;
bool allOn=false;
bool cycle=false;
/*
//ATT: On the Uno and other ATMEGA based boards, unsigned ints (unsigned integers) are the same as ints in that they store a 2 byte value.
//Long variables are extended size variables for number storage, and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.
/*******************************************************************/
void setup() {
  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_in, OUTPUT);
  pinMode(VFD_ce, OUTPUT);
  //initialize the LED pin as an output:
  pinMode(13, OUTPUT);
  //pinMode(ledPin, OUTPUT);
  //initialize the pushbutton pin as an input:
  //pinMode(buttonPin, INPUT);  //Next line is the attach of interruption to pin 2
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  //
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                buttonReleasedInterrupt,
                FALLING);
  //
  Serial.begin(115200); // only to debug  
  Serial.println("You reach a Reset Hardware!");               
}
/*******************************************************************/
// I h've created 3 functions to send bit's, one with strobe, other without strobe and one with first byte with strobe followed by remaing bits.
void send_char_without(unsigned char a){
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  data=a;
      for (mask = 0B00000001; mask > 0; mask <<= 1) { //iterate through bit mask
      digitalWrite(VFD_clk, LOW);
      delayMicroseconds(2);
          if (data & mask){ // if bitwise AND resolves to true
            digitalWrite(VFD_in, HIGH);
            //Serial.print(1);
          }
          else{ //if bitwise and resolves to false
            digitalWrite(VFD_in, LOW);
            //Serial.print(0);
          }
      digitalWrite(VFD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
      delayMicroseconds(2);
      }
}
/*******************************************************************/
void send_addr(unsigned char a){
    unsigned char data = 170; //value to transmit, binary 10101010
    unsigned char mask = 1; //our bitmask
    data=a;
    digitalWrite(VFD_ce, LOW); //
    delayMicroseconds(2);
      for (mask = 0B00000001; mask > 0; mask <<= 1) { //iterate through bit mask
      digitalWrite(VFD_clk, LOW);
      delayMicroseconds(2);
              if (data & mask){ // if bitwise AND resolves to true
                digitalWrite(VFD_in, HIGH);
                //Serial.print(1);
              }
              else{ //if bitwise and resolves to false
                digitalWrite(VFD_in, LOW);
                //Serial.print(0);
              }
      digitalWrite(VFD_clk,HIGH);// 
      delayMicroseconds(1);
    } 
    //On this model we rise the CE with clock already at High value, after send address 0x41!
    delayMicroseconds(2);
    digitalWrite(VFD_ce, HIGH);
    //Serial.println();
}
/*******************************************************************/
void allON(){
  //The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
  
  send_addr(addr); //(0x41) firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
  //
  // On the 75826 of Sony 
  send_char_without(0B11111111);  send_char_without(0B11111111); //  8:1   -16:9
  send_char_without(0B11111111);  send_char_without(0B11111111); // 24:17  -32:25
  send_char_without(0B11111111);  send_char_without(0B11111111); // 40:33  -48:41
  send_char_without(0B11111111);  send_char_without(0B10001000); // 53:49
  send_char_without(0B00000000); // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
  // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
  digitalWrite(VFD_ce, LOW); //
  delayMicroseconds(1);
  send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
  //
  // On the 75826 of Sony 
  send_char_without(0B11111111);  send_char_without(0B11111111); // 60:53  -68:61
  send_char_without(0B11111111);  send_char_without(0B11111111); // 76:69  -84:77
  send_char_without(0B11111111);  send_char_without(0B11111111); // 92:85 -100:93
  send_char_without(0B11111111);  send_char_without(0B00000000); //104:101
  send_char_without(0B10000000);    // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
  // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
  digitalWrite(VFD_ce, LOW); //
  delayMicroseconds(1);
  send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
  //
  // On the 75826 of Sony 
  send_char_without(0B11111111);  send_char_without(0B11111111); // 112:105  -120:113
  send_char_without(0B11111111);  send_char_without(0B11111111); // 128:121  -136:129
  send_char_without(0B11111111);  send_char_without(0B11111111); // 144:137  -152:145
  send_char_without(0B11111111);  send_char_without(0B00000000); // 156:153   
  send_char_without(0B01000000); // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
  // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
  digitalWrite(VFD_ce, LOW); //
  delayMicroseconds(1);
  send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
  //
  // On the 75826 of Sony 
  send_char_without(0B11111111);  send_char_without(0B11111111); // 164:157  -172:165
  send_char_without(0B11111111);  send_char_without(0B11111111); // 180:173  -188:181
  send_char_without(0B11111111);  send_char_without(0B11111111); // 196:189  -204:197
  send_char_without(0B11111111);  send_char_without(0B00000000);
  send_char_without(0B11000000); //  P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
  // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
  delayMicroseconds(1);
  digitalWrite(VFD_ce, LOW); // 
  delayMicroseconds(1);
}
void allOFF(){
  //The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
  digitalWrite(VFD_ce, LOW); //
  delayMicroseconds(1);
  send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
  //
  // On the 75826 of Sony 
  send_char_without(0B00010001);  send_char_without(0B00000000); //  8:1   -16:9
  send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25
  send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41
  send_char_without(0B00000000);  send_char_without(0B10001000);
  send_char_without(0B00000000); //  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
  // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
  delayMicroseconds(1);
  digitalWrite(VFD_ce, LOW); // 
  delayMicroseconds(1);
  //
  send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
  //
  // On the 75826 of Sony 
  send_char_without(0B00000000);  send_char_without(0B00000000); // 60:53  -68:61
  send_char_without(0B00000000);  send_char_without(0B00000000); // 76:69  -84:77
  send_char_without(0B00000000);  send_char_without(0B00000000); // 92:85 -100:93
  send_char_without(0B00000000);  send_char_without(0B00000000);  //104:101
  send_char_without(0B10000000);    // // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
  // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
  delayMicroseconds(1);
  digitalWrite(VFD_ce, LOW); // 
  delayMicroseconds(1);
  //
  send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
  //
  // On the 75826 of Sony 
  send_char_without(0B00000000);  send_char_without(0B00000000); // 112:105  -120:113
  send_char_without(0B00000000);  send_char_without(0B00000000); // 128:121  -136:129
  send_char_without(0B00000000);  send_char_without(0B00000000); // 144:137  -152:145
  send_char_without(0B00000000);  send_char_without(0B00000000); // 156:153
  send_char_without(0B01000000);   // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
  // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
  delayMicroseconds(1);
  digitalWrite(VFD_ce, LOW); // 
  delayMicroseconds(1);
  //
  send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
  //
  // On the 75826 of Sony 
  send_char_without(0B00000000);  send_char_without(0B00000000); // 164:157  -172:165
  send_char_without(0B00000000);  send_char_without(0B00000000); // 180:173  -188:181
  send_char_without(0B00000000);  send_char_without(0B00000000); // 196:189  -204:197
  send_char_without(0B00000000);  send_char_without(0B00000000);
  send_char_without(0B11000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
  // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
  delayMicroseconds(1);
  digitalWrite(VFD_ce, LOW); // 
  delayMicroseconds(1);
}
void msgHiFolks(){
    //The p0, p1, p2 & p3 at 0, means all pins from s1 to s8 will belongs to segments, other's settings tell will px is a port general purpose!
    digitalWrite(VFD_ce, LOW); //
    delayMicroseconds(1);
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony This block send a block of 52 bits
    send_char_without(0B00010001);  send_char_without(0B00000000); //  8:1   -16:9  //Bit 5 is used as GPIO2 and is control of Dimmer!!!
    send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41 
    send_char_without(0B00000000);  send_char_without(0B10001000); // 56:49  -64:57  //Please pay attention bit 64(DN) it define 200 or 208 segments
    send_char_without(0B00000000); // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony This block send a block of 52 bits
    send_char_without(0B00000000);  send_char_without(0B00000000); // 64:57  -72:65 
    send_char_without(0B10010110);  send_char_without(0B00001000); // 80:73  -88:81  
    send_char_without(0B10100000);  send_char_without(0B11100000); // 96:89 -104:97 
    send_char_without(0B00000000);  send_char_without(0B00000000); //112:105 
    send_char_without(0B10000000); // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony This block send a block of 48 bits
    send_char_without(0B00000000);  send_char_without(0B10101000); // 120:113  -128:121//  
    send_char_without(0B00001010);  send_char_without(0B10101001); // 136:129  -144:137//  
    send_char_without(0B00000100);  send_char_without(0B11100001); // 152:145  -160:153// 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 168:161
    send_char_without(0B01000000); //  P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony This block send a block of 56 bits
    send_char_without(0B00000000);  send_char_without(0B00000000); // 176:169  -184:177// 
    send_char_without(0B00000000);  send_char_without(0B10010100); // 192:185  -200:193//  
    send_char_without(0B11100000);  send_char_without(0B00000000); // 208:201  -216:209// 
    send_char_without(0B00001110);  send_char_without(0B00000000); // 224:217
    send_char_without(0B11000010); //  P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
}
void msgSONY(){
    //The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
    digitalWrite(VFD_ce, LOW); //
    delayMicroseconds(1);
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony 
    send_char_without(0B00010001);  send_char_without(0B00000000); //  8:1   -16:9  
    send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41 
    send_char_without(0B00000000);  send_char_without(0B10001000); // 56:49  -64:57  //Please pay attention bit 64(DN) it define 200 or 208 segments 
    send_char_without(0B00000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 64:57  -72:65 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 80:73  -88:81  
    send_char_without(0B00000000);  send_char_without(0B00000000); // 96:89 -104:97 
    send_char_without(0B00000000);  send_char_without(0B00000000); //112:105 
    send_char_without(0B10000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 120:113  -128:121//  
    send_char_without(0B00000000);  send_char_without(0B00000000); // 136:129  -144:137//  
    send_char_without(0B00100000);  send_char_without(0B00000110); // 152:145  -160:153// 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 168:161
    send_char_without(0B01000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony 
    send_char_without(0B10100000);  send_char_without(0B00101000); // 176:169  -184:177// 
    send_char_without(0B10101010);  send_char_without(0B10010000); // 192:185  -200:193//  
    send_char_without(0B00001010);  send_char_without(0B10011000); // 208:201  -216:209// 
    send_char_without(0B00000110);  send_char_without(0B00000000); // 224:217
    send_char_without(0B11000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
}
void msgCDX(){
    //The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
    digitalWrite(VFD_ce, LOW); //
    delayMicroseconds(1);
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony 
    send_char_without(0B00010001);  send_char_without(0B00000000); //  8:1   -16:9  
    send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41 
    send_char_without(0B00000000);  send_char_without(0B10001000); // 56:49   
    send_char_without(0B00000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 64:57  -72:65 
    send_char_without(0B10011010);  send_char_without(0B10100000); // 80:73  -88:81  
    send_char_without(0B00001100);  send_char_without(0B01101001); // 96:89 -104:97 
    send_char_without(0B00000000);  send_char_without(0B00000000); //112:105 
    send_char_without(0B10000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony 
    send_char_without(0B00000110);  send_char_without(0B11001001); // 120:113  -128:121//  
    send_char_without(0B00001110);  send_char_without(0B11100001); // 136:129  -144:137//  
    send_char_without(0B00000100);  send_char_without(0B01000000); // 152:145  -160:153// 
    send_char_without(0B00000000);  send_char_without(0B00000000); // 168:161
    send_char_without(0B01000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
    //
    send_addr(addr); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
    //
    // On the 75826 of Sony 
    send_char_without(0B00000000);  send_char_without(0B01101010); // 176:169  -184:177// 
    send_char_without(0B10100000);  send_char_without(0B10010100); // 192:185  -200:193//  
    send_char_without(0B00000000);  send_char_without(0B10010000); // 208:201  -216:209// 
    send_char_without(0B00001010);  send_char_without(0B00000000); // 224:217
    send_char_without(0B11000000);  // P0 P1 P2 P3 DR DN FC1 FC2 OC SC FC0 BU 0 0; Last 2 bits is "DD" data direction, and is used
    // to mark the 4 groups of 64 bits, 00, 01, 10, 11.
    delayMicroseconds(1);
    digitalWrite(VFD_ce, LOW); // 
    delayMicroseconds(1);
}
void searchOfSegments(){
  int group = 0x00;
  byte nBit =0x00;
  byte nMask = 0b00000001;
  unsigned int block =0;
  unsigned int nSeg=0x00;  
  Serial.println();
  Serial.println("We start the test of segments!");
  for(block = 0; block < 4; block++){  //This is the last 2 bit's marked as DD, group: 0x00, 0x01, 0x10, 0x11;
    for( group = 0; group < 8; group++){   // Do until n bits 15*8 bits
      //for(int nBit=0; nBit<8; nBit++){
        for (nMask = 0b00000001; nMask > 0; nMask <<= 1){
          Aa=0x00; Ab=0x00; Ac=0x00; Ad=0x00; Ae=0x00; Af=0x00; Ag=0x00;  Ah=0x00;
                switch (group){
                  case 0: Aa=nMask; break;
                  case 1: Ab=nMask; break;
                  case 2: Ac=nMask; break;
                  case 3: Ad=nMask; break;
                  case 4: Ae=nMask; break;
                  case 5: Af=nMask; break;
                  case 6: Ag=nMask; break;
                  case 7: Ah=nMask; break;
                }
          
        nSeg++;
        if((nSeg >= 0) && (nSeg < 56)){
          blockBit=0;
          }
          if((nSeg >= 57) && (nSeg < 112)){
          blockBit=1;
          }
          if((nSeg >= 112) && (nSeg < 168)){
          blockBit=2;
          }
          if((nSeg >= 169) && (nSeg < 224)){
          blockBit=3;
          }
        
          if (nSeg >= 224){
            nSeg=0;
            group=0;
            block=0;
            break;
          }
        
    //This start the control of button to allow continue teste! 
                    while(1){
                          if(!buttonReleased){
                            delay(200);
                          }
                          else{
                            delay(15);
                            buttonReleased = false;
                            break;
                            }
                      }
        delay(50);

        

        segments();
          Serial.print(nSeg, DEC);Serial.print(", BlockBit: ");Serial.print(blockBit, HEX);Serial.print(", group: ");Serial.print(group, DEC);Serial.print(", nMaskBit: "); Serial.print(nMaskBit, DEC);Serial.print("   \t");
          Serial.print(Ah, HEX);Serial.print(", ");Serial.print(Ag, HEX);Serial.print(", ");Serial.print(Af, HEX);Serial.print(", ");Serial.print(Ae, HEX);Serial.print(", ");
          Serial.print(Ad, HEX);Serial.print(", ");Serial.print(Ac, HEX);Serial.print(", ");Serial.print(Ab, HEX);Serial.print(", ");Serial.print(Aa, HEX); Serial.print("; ");
          //
            nMaskBit++;  //Here I increment the number to represent the mask decimal value
                        if(nMaskBit >= 8){
                          nMaskBit =0;
                    }
          Serial.println();
          delay (250);   
          }      
        }        
    }
}
void segments(){
  //Bit function: 
  digitalWrite(VFD_ce, LOW); //
  delayMicroseconds(1);
  send_addr(addr); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
  delayMicroseconds(1);
  // 
      send_char_without(~Aa);  send_char_without(~Ab);  //   8:1     -16:9// 
      send_char_without(~Ac);  send_char_without(~Ad);  //  24:17    -32:25//
      send_char_without(~Ae);  send_char_without(~Af);  //  40:33    -48:41// 
      send_char_without(~Ag);  send_char_without(~Ah);  //  52:49     //
  //   
      //The next switch finalize the burst of bits  
          switch (blockBit){ //Last 2 bits is "DD" data direction, and is used to mark the 4 groups of 36 bits, 00, 01, 10, 11.                                 
            case 0: send_char_without(0B00000000); break; //Block 00
            case 1: send_char_without(0B10000000); break; //Block 01
            case 2: send_char_without(0B01000000); break; //Block 10
            case 3: send_char_without(0B11000000); break; //Block 11
          }
  delayMicroseconds(1);
  digitalWrite(VFD_ce, LOW); //                   
}
void loop() {
  for(uint8_t s = 0; s < 4; s++){
      allOFF();
      delay(750);
      allON(); // All on
      delay(750);
  }
  msgHiFolks();
  delay(1200);
  msgSONY();
  delay(1200);
  msgCDX();
  delay(1200);
  allOFF();
  delay(1200);
  allON(); // All on
  delay(2000);
    searchOfSegments(); //Uncomment this line if you want run the find segments. 
    //This need a button to GND (with a 2k) at pin 2 of Arduino already implemented on code!  
}
void buttonReleasedInterrupt() {
buttonReleased = true; // This is the line of interrupt button to advance one step on the search of segments!
}
