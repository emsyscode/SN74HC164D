//                  0x40
//                 ------
//        0x01    |      |  0x80
//                | 0x02 |
//                 ------
//        0x08    |      |  0x01
//                | 0x02 |
//                 ------  o 0x01
//
//This is the assignment of the bit weight to the segments.
//Your digits should be created based on these values.
//When sent 0x00 this means all segments will be ON because the
//display is anode commum.
//the weight of bit at binary format: 0b84218421

#define tr1 8
#define tr2 9
#define tr3 10
#define tr4 11

#define dataIn 4
#define clk 5

//This table is filled inverted is necessary apply the "~" to inverte it!
char twoPoints = 0b11111110;

//  //.................bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch
//  char test[10] = {0b11011110, 0b10000010, 0b01110110, 0b11100110, 0b10110010, 0b01111110, 0b11000010, 0b11111110, 0b11110110, 0b11111111};

//....................bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch
uint8_t test[11] = {0b00100001, 0b01111101, 0b00010011, 0b00011001, 0b01001101, 0b10001001, 0b10000001, 0b00111101, 0b00000001, 0b00001001, 0b11111111 };

//........................bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch....bagfedch
uint8_t msgHELLO[13] = {0b11111111, 0b11111111, 0b11111111, 0b01000101, 0b10000011, 0b11100011, 0b11100011, 0b00100001, 0b11111111, 0b11111111, 0b11111111, 0b11111111 };
//The table is to be used with "~" and is in Hexadecimal format to stay more short but is not filled to this panel, is necessary adapt it!
//char numbers[10] = {0xE7, 0x24, 0xAB, 0xAD, 0x6C, 0xCD, 0xCF, 0xA4, 0xEF, 0xED};
//char twoPoints = 0x01;

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(115200);
}
void serialData(uint8_t val){
  unsigned char data = 0x00;
  unsigned char mask = 1; //our bitmask
  data=val;
      for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
      digitalWrite(clk, LOW);
      delayMicroseconds(1);
        if (data & mask){ // if bitwise AND resolves to true
          digitalWrite(dataIn, HIGH);
          Serial.print(1); //Only to debug
        }
        else{ //if bitwise and resolves to false
          digitalWrite(dataIn, LOW);
          Serial.print(0); //Only to debug
        }
        digitalWrite(clk, HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
        delayMicroseconds(1);
      }
  digitalWrite(clk, LOW);
  Serial.println(); //Only to debug
}
void sendDigit(uint8_t grid, uint8_t digit){
  switch (grid){
          case 1: digitalWrite(8, LOW); digitalWrite(9, HIGH); digitalWrite(10, HIGH); digitalWrite(11, HIGH);
                  serialData((digit)); break;
          case 2: digitalWrite(8, HIGH); digitalWrite(9, LOW); digitalWrite(10, HIGH); digitalWrite(11, HIGH);
                  serialData((digit)); break;
          case 3: digitalWrite(8, HIGH); digitalWrite(9, HIGH); digitalWrite(10, LOW); digitalWrite(11, HIGH);
                  serialData((digit)); break;
          case 4: digitalWrite(8, HIGH); digitalWrite(9, HIGH); digitalWrite(10, HIGH); digitalWrite(11, LOW);
                  serialData((digit)); break;
        }
}
void showStatic(){
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
      for (uint8_t i = 0; i<10; i++){
      serialData(test[i]); 
      delay(800);
      }
}
void msgStatic(){
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
      for (uint8_t i = 0; i<11; i++){
      sendDigit(1, test[i]);
      sendDigit(2, test[i]);
      sendDigit(3, test[i]);
      sendDigit(4, test[i]); 
      delay(80);
      }
}
void showDinamicBlinking(){
  for (uint8_t s = 0; s<250; s++){
        sendDigit(1, test[8]);
        delay(10);
        sendDigit(2, test[3]);
        delay(10);
        sendDigit(3, test[1]);
        delay(10);
        sendDigit(4, test[2]);
        delay(10);
      }
}
void showDinamic(){
  for (uint8_t s = 0; s<250; s++){
        sendDigit(1, test[8]);
        delay(5);
        sendDigit(2, test[3]);
        delay(5);
        sendDigit(3, test[1]);
        delay(5);
        sendDigit(4, test[2]);
        delay(5);
      }
}
void text(){
 //This block here is responsable to control the base of 4 transistors to polarize the display
 // and set the time when the message letter will be present and where it is
 // digit that will be used in the visualization of the character.
  uint8_t pos = 0x00;
  for(uint8_t p = 0; p< 9; p++){
        for(uint8_t i = 0; i<50; i++){
          pos = p;
          sendDigit(4, msgHELLO[pos]);
          delay(5);
          pos = p + 1;
          sendDigit(3, msgHELLO[pos]);
          delay(5);
          pos = p + 2;
          sendDigit(2, msgHELLO[pos]);
          delay(5);
          pos = p + 3;
          sendDigit(1, msgHELLO[pos]);
          delay(5);
        }
    }
}
void loop() {
   text();
   msgStatic();
   showStatic();
   showDinamic();
   showDinamicBlinking();
}
